import type { ReactNode } from 'react';

/* Type expressions */
export type TypeExpr =
  | { kind: 'primitive', name: string }
  | { kind: 'literal', value: string | number | boolean }
  | { kind: 'ref', name: string }
  | { kind: 'union', of: TypeExpr[] }
  | { kind: 'array', of: TypeExpr }
  | { kind: 'object', fields: Field[] };

export const t = {
  string: { kind: 'primitive', name: 'string' } as TypeExpr,
  number: { kind: 'primitive', name: 'number' } as TypeExpr,
  boolean: { kind: 'primitive', name: 'boolean' } as TypeExpr,
  lit: (value: string | number | boolean): TypeExpr => ({ kind: 'literal', value }),
  ref: (name: string): TypeExpr => ({ kind: 'ref', name }),
  union: (...of: TypeExpr[]): TypeExpr => ({ kind: 'union', of }),
  array: (of: TypeExpr): TypeExpr => ({ kind: 'array', of }),
  object: (fields: Field[]): TypeExpr => ({ kind: 'object', fields }),
};

/* Type definitions */
export type Field = {
  name: string;
  type: TypeExpr;
  optional?: boolean;
  description?: string;
};

type TypeDefBase = {
  name: string;
  description?: ReactNode;
  example?: unknown;
};

export type TypeDef = TypeDefBase & (
  | { kind: 'struct', fields: Field[] }
  | { kind: 'enum', values: { value: string, ref?: string, description?: string }[] }
  | { kind: 'union', of: string[], discriminator?: string }
);

/* Route definitions */
export type RoutePayload = {
  type: TypeExpr;
  example?: unknown;
};

export type RouteDef = {
  method: 'GET' | 'POST' | 'PUT' | 'PATCH' | 'DELETE';
  path: string;
  websocket?: boolean;
  summary: string;
  description?: ReactNode;
  params?: Field[];
  response?: RoutePayload;
  messages?: RoutePayload;
  errors?: { status: number, description: string }[];
  usage?: string;
};

const typeAnchor = (name: string) => `type-${name}`;
const routeAnchor = (route: RouteDef) => `route-${route.method}${route.path.replace(/[^a-zA-Z0-9]+/g, '-')}`.replace(/-+$/, '');

/* Syntax colors */
const C = {
  punct: 'text-gray-500 dark:text-gray-400',
  key: 'text-sky-700 dark:text-sky-300',
  string: 'text-emerald-700 dark:text-emerald-300',
  number: 'text-amber-700 dark:text-amber-300',
  primitive: 'text-purple-700 dark:text-purple-300',
  comment: 'text-gray-400 dark:text-gray-500 italic',
};

export const TypeRef = ({ name }: { name: string }) => (
  <a href={`#${typeAnchor(name)}`} className="text-blue-600 dark:text-blue-400 underline decoration-dotted underline-offset-2 hover:decoration-solid">
    {name}
  </a>
);

function literalText(value: string | number | boolean) {
  return typeof value === 'string' ? JSON.stringify(value) : String(value);
}

function exprText(expr: TypeExpr): string {
  switch (expr.kind) {
  case 'primitive': return expr.name;
  case 'literal': return literalText(expr.value);
  case 'ref': return expr.name;
  case 'union': return expr.of.map(exprText).join(' | ');
  case 'array': return expr.of.kind === 'union' ? `(${exprText(expr.of)})[]` : `${exprText(expr.of)}[]`;
  case 'object': return '{ … }';
  }
}

/* Single-line rendering of a type expression */
function Expr({ expr }: { expr: TypeExpr }) {
  switch (expr.kind) {
  case 'primitive':
    return <span className={C.primitive}>{expr.name}</span>;
  case 'literal':
    return <span className={typeof expr.value === 'string' ? C.string : C.number}>{literalText(expr.value)}</span>;
  case 'ref':
    return <TypeRef name={expr.name}/>;
  case 'union':
    return <>{expr.of.map((e, i) => <span key={i}>{i > 0 && <span className={C.punct}> | </span>}<Expr expr={e}/></span>)}</>;
  case 'array': {
    const parens = expr.of.kind === 'union';
    return <>{parens && <span className={C.punct}>(</span>}<Expr expr={expr.of}/><span className={C.punct}>{parens ? ')[]' : '[]'}</span></>;
  }
  case 'object':
    return <span className={C.punct}>{'{ … }'}</span>;
  }
}

function CodeBlock({ label, children }: { label: string, children: ReactNode }) {
  return (
    <div className="flex flex-col rounded-md border border-gray-200 dark:border-gray-700 overflow-hidden">
      <div className="px-3 py-1 text-xs uppercase tracking-wide text-gray-500 bg-gray-50 dark:bg-gray-800 border-b border-gray-200 dark:border-gray-700">{label}</div>
      <pre className="flex-1 p-3 m-0 text-sm leading-6 font-mono overflow-x-auto bg-white dark:bg-gray-900">{children}</pre>
    </div>
  );
}

/* A rendered line of a shape; width is its length in characters, used to align comments */
type Line = { width: number, node: ReactNode, comment?: ReactNode };

function Lines({ lines }: { lines: Line[] }) {
  const width = Math.max(...lines.filter(l => l.comment).map(l => l.width), 0);
  return (
    <>
      {lines.map((l, i) => (
        <div key={i}>
          {l.node}
          {l.comment && <span className={C.comment}>{' '.repeat(width - l.width + 2)}// {l.comment}</span>}
        </div>
      ))}
    </>
  );
}

function objectLines(fields: Field[], indent: number, suffix: string): Line[] {
  const pad = '  '.repeat(indent);
  const innerPad = '  '.repeat(indent + 1);
  const lines: Line[] = [{ width: pad.length + 1, node: <>{pad}<span className={C.punct}>{'{'}</span></> }];

  fields.forEach((f, i) => {
    const comma = i < fields.length - 1 ? ',' : '';
    const key = `"${f.name}"${f.optional ? '?' : ''}`;
    const head = <>{innerPad}<span className={C.key}>{key}</span><span className={C.punct}>: </span></>;
    const headWidth = innerPad.length + key.length + 2;

    if (f.type.kind === 'object') {
      const inner = objectLines(f.type.fields, indent + 1, comma);
      inner[0] = { width: headWidth + 1, node: <>{head}<span className={C.punct}>{'{'}</span></>, comment: f.description };
      lines.push(...inner);
    } else {
      lines.push({
        width: headWidth + exprText(f.type).length + comma.length,
        node: <>{head}<Expr expr={f.type}/><span className={C.punct}>{comma}</span></>,
        comment: f.description,
      });
    }
  });

  lines.push({ width: pad.length + 1 + suffix.length, node: <>{pad}<span className={C.punct}>{'}' + suffix}</span></> });
  return lines;
}

function Shape({ expr }: { expr: TypeExpr }) {
  if (expr.kind === 'object')
    return <Lines lines={objectLines(expr.fields, 0, '')}/>;
  return <div><Expr expr={expr}/></div>;
}

function EnumShape({ values }: { values: { value: string, ref?: string, description?: string }[] }) {
  const lines = values.map((v, i): Line => {
    const prefix = i === 0 ? '  ' : '| ';
    const text = JSON.stringify(v.value);
    return {
      width: prefix.length + text.length,
      node: <><span className={C.punct}>{prefix}</span><span className={C.string}>{text}</span></>,
      comment: v.ref ? <>→ <TypeRef name={v.ref}/></> : v.description,
    };
  });
  return <Lines lines={lines}/>;
}

function UnionShape({ of }: { of: string[] }) {
  return (
    <>
      {of.map((name, i) => (
        <div key={name}><span className={C.punct}>{i === 0 ? '  ' : '| '}</span><TypeRef name={name}/></div>
      ))}
    </>
  );
}

/* Example JSON with syntax colors */
function JsonValue({ value, indent }: { value: unknown, indent: number }): ReactNode {
  const pad = '  '.repeat(indent);
  if (value === null) return <span className={C.primitive}>null</span>;
  if (typeof value === 'string') return <span className={C.string}>{JSON.stringify(value)}</span>;
  if (typeof value === 'number' || typeof value === 'boolean') return <span className={C.number}>{String(value)}</span>;
  if (Array.isArray(value)) {
    if (value.length === 0) return <span className={C.punct}>[]</span>;
    return (
      <>
        <span className={C.punct}>[</span>{'\n'}
        {value.map((v, i) => <span key={i}>{pad}{'  '}<JsonValue value={v} indent={indent + 1}/>{i < value.length - 1 && <span className={C.punct}>,</span>}{'\n'}</span>)}
        {pad}<span className={C.punct}>]</span>
      </>
    );
  }
  const entries = Object.entries(value as object);
  if (entries.length === 0) return <span className={C.punct}>{'{}'}</span>;
  return (
    <>
      <span className={C.punct}>{'{'}</span>{'\n'}
      {entries.map(([k, v], i) => (
        <span key={k}>{pad}{'  '}<span className={C.key}>{JSON.stringify(k)}</span><span className={C.punct}>: </span><JsonValue value={v} indent={indent + 1}/>{i < entries.length - 1 && <span className={C.punct}>,</span>}{'\n'}</span>
      ))}
      {pad}<span className={C.punct}>{'}'}</span>
    </>
  );
}

function ShapeWithExample({ shapeLabel, shape, example }: { shapeLabel: string, shape: ReactNode, example?: unknown }) {
  return (
    <div className={`grid gap-3 ${example !== undefined ? 'md:grid-cols-2' : ''}`}>
      <CodeBlock label={shapeLabel}>{shape}</CodeBlock>
      {example !== undefined && (
        <CodeBlock label="Example">
          <JsonValue value={example} indent={0}/>
        </CodeBlock>
      )}
    </div>
  );
}

function AnchorLink({ anchor, label }: { anchor: string, label: string }) {
  return <a href={`#${anchor}`} className="text-gray-400 opacity-0 group-hover:opacity-100 hover:text-blue-500" aria-label={`Link to ${label}`}>#</a>;
}

function Badge({ className, children }: { className: string, children: ReactNode }) {
  return <span className={`text-xs font-medium px-2 py-0.5 rounded-full ${className}`}>{children}</span>;
}

function SubHeading({ children }: { children: ReactNode }) {
  return <h4 className="mt-4 mb-2 text-sm font-semibold uppercase tracking-wide text-gray-500">{children}</h4>;
}

const KIND_STYLES: Record<TypeDef['kind'], string> = {
  struct: 'bg-sky-100 text-sky-800 dark:bg-sky-900/50 dark:text-sky-200',
  enum: 'bg-amber-100 text-amber-800 dark:bg-amber-900/50 dark:text-amber-200',
  union: 'bg-purple-100 text-purple-800 dark:bg-purple-900/50 dark:text-purple-200',
};

export function TypeCard({ def }: { def: TypeDef }) {
  const anchor = typeAnchor(def.name);
  return (
    <section id={anchor} className="doc scroll-mt-4 mb-6 rounded-lg border border-gray-200 dark:border-gray-700 p-5 target:ring-2 target:ring-blue-400">
      <header className="flex items-center gap-3 mb-3 group">
        <h3 className="text-lg font-semibold font-mono m-0">{def.name}</h3>
        <Badge className={KIND_STYLES[def.kind]}>{def.kind}</Badge>
        <AnchorLink anchor={anchor} label={def.name}/>
      </header>
      {def.description && <div className="mb-3 text-gray-700 dark:text-gray-300 space-y-2">{def.description}</div>}
      {def.kind === 'union' && def.discriminator && (
        <p className="mb-3 text-sm text-gray-600 dark:text-gray-400">
          Discriminated by the <code className="font-mono">{def.discriminator}</code> field.
        </p>
      )}
      <ShapeWithExample
        shapeLabel="Shape"
        example={def.example}
        shape={<>
          {def.kind === 'struct' && <Shape expr={t.object(def.fields)}/>}
          {def.kind === 'enum' && <EnumShape values={def.values}/>}
          {def.kind === 'union' && <UnionShape of={def.of}/>}
        </>}
      />
    </section>
  );
}

const METHOD_STYLES: Record<RouteDef['method'], string> = {
  GET: 'bg-green-100 text-green-800 dark:bg-green-900/50 dark:text-green-200',
  POST: 'bg-blue-100 text-blue-800 dark:bg-blue-900/50 dark:text-blue-200',
  PUT: 'bg-amber-100 text-amber-800 dark:bg-amber-900/50 dark:text-amber-200',
  PATCH: 'bg-amber-100 text-amber-800 dark:bg-amber-900/50 dark:text-amber-200',
  DELETE: 'bg-red-100 text-red-800 dark:bg-red-900/50 dark:text-red-200',
};

function RoutePath({ path }: { path: string }) {
  return (
    <>
      {path.split(/(\{[^}]+\})/).map((part, i) => (
        part.startsWith('{')
          ? <span key={i} className="text-amber-700 dark:text-amber-300">{part}</span>
          : <span key={i}>{part}</span>
      ))}
    </>
  );
}

function routeUrl(route: RouteDef, baseUrl: string) {
  return (route.websocket ? baseUrl.replace(/^http/, 'ws') : baseUrl) + route.path;
}

export function RouteCard({ route, baseUrl }: { route: RouteDef, baseUrl: string }) {
  const anchor = routeAnchor(route);
  return (
    <section id={anchor} className="doc scroll-mt-4 mb-6 rounded-lg border border-gray-200 dark:border-gray-700 p-5 target:ring-2 target:ring-blue-400">
      <header className="flex items-center gap-3 group">
        <Badge className={METHOD_STYLES[route.method]}>{route.method}</Badge>
        <h3 className="text-lg font-semibold font-mono m-0"><RoutePath path={route.path}/></h3>
        {route.websocket && <Badge className="bg-purple-100 text-purple-800 dark:bg-purple-900/50 dark:text-purple-200">WebSocket</Badge>}
        <AnchorLink anchor={anchor} label={`${route.method} ${route.path}`}/>
      </header>
      <p className="mt-1 mb-3 font-mono text-sm text-gray-500 break-all">{routeUrl(route, baseUrl)}</p>
      <p className="mb-2 text-gray-700 dark:text-gray-300">{route.summary}</p>
      {route.description && <div className="mb-3 text-gray-700 dark:text-gray-300 space-y-2">{route.description}</div>}

      {route.params && route.params.length > 0 && (
        <>
          <SubHeading>Path parameters</SubHeading>
          <table className="w-full text-sm">
            <tbody>
              {route.params.map(p => (
                <tr key={p.name} className="border-t border-gray-200 dark:border-gray-700">
                  <td className="py-2 pr-4 align-top font-mono whitespace-nowrap">{p.name}</td>
                  <td className="py-2 pr-4 align-top font-mono whitespace-nowrap"><Expr expr={p.type}/></td>
                  <td className="py-2 align-top text-gray-700 dark:text-gray-300">{p.description}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </>
      )}

      {route.response && (
        <>
          <SubHeading>Response</SubHeading>
          <ShapeWithExample shapeLabel="Body" shape={<Shape expr={route.response.type}/>} example={route.response.example}/>
        </>
      )}

      {route.messages && (
        <>
          <SubHeading>Messages</SubHeading>
          <ShapeWithExample shapeLabel="Each message" shape={<Shape expr={route.messages.type}/>} example={route.messages.example}/>
        </>
      )}

      {route.errors && route.errors.length > 0 && (
        <>
          <SubHeading>Errors</SubHeading>
          <table className="w-full text-sm">
            <tbody>
              {route.errors.map(e => (
                <tr key={e.status} className="border-t border-gray-200 dark:border-gray-700">
                  <td className="py-2 pr-4 align-top font-mono whitespace-nowrap text-red-700 dark:text-red-300">{e.status}</td>
                  <td className="py-2 align-top text-gray-700 dark:text-gray-300">{e.description}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </>
      )}

      {route.usage && (
        <>
          <SubHeading>Usage</SubHeading>
          <CodeBlock label="JavaScript">{route.usage.trim()}</CodeBlock>
        </>
      )}
    </section>
  );
}

const indexLinkClass = 'font-mono text-sm px-2 py-1 rounded border border-gray-200 dark:border-gray-700 hover:bg-gray-100 dark:hover:bg-gray-800';

export function TypeIndex({ defs }: { defs: TypeDef[] }) {
  return (
    <nav className="mb-8 flex flex-wrap gap-2">
      {defs.map(d => (
        <a key={d.name} href={`#${typeAnchor(d.name)}`} className={indexLinkClass}>
          {d.name}
        </a>
      ))}
    </nav>
  );
}

export function RouteIndex({ routes }: { routes: RouteDef[] }) {
  return (
    <nav className="mb-8 flex flex-wrap gap-2">
      {routes.map(r => (
        <a key={routeAnchor(r)} href={`#${routeAnchor(r)}`} className={indexLinkClass}>
          {r.method} {r.path}
        </a>
      ))}
    </nav>
  );
}
