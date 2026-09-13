import type { ReactNode } from 'react';

/* Type expressions */
export type TypeExpr =
  | { kind: 'primitive', name: string }
  | { kind: 'literal', value: string | number | boolean }
  | { kind: 'ref', name: string }
  | { kind: 'union', of: TypeExpr[] };

export const t = {
  string: { kind: 'primitive', name: 'string' } as TypeExpr,
  number: { kind: 'primitive', name: 'number' } as TypeExpr,
  boolean: { kind: 'primitive', name: 'boolean' } as TypeExpr,
  lit: (value: string | number | boolean): TypeExpr => ({ kind: 'literal', value }),
  ref: (name: string): TypeExpr => ({ kind: 'ref', name }),
  union: (...of: TypeExpr[]): TypeExpr => ({ kind: 'union', of }),
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

const typeAnchor = (name: string) => `type-${name}`;

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
  }
}

function Expr({ expr }: { expr: TypeExpr }) {
  switch (expr.kind) {
  case 'primitive':
    return <span className={C.primitive}>{expr.name}</span>;
  case 'literal':
    return <span className={typeof expr.value === 'string' ? C.string : C.number}>{literalText(expr.value)}</span>;
  case 'ref':
    return <TypeRef name={expr.name}/>;
  case 'union':
    return <>{expr.of.map((e, i) => <>{i > 0 && <span className={C.punct}> | </span>}<Expr expr={e}/></>)}</>;
  }
}

function CodeBlock({ label, children }: { label: string, children: ReactNode }) {
  return (
    <div className="rounded-md border border-gray-200 dark:border-gray-700 overflow-hidden">
      <div className="px-3 py-1 text-xs uppercase tracking-wide text-gray-500 bg-gray-50 dark:bg-gray-800 border-b border-gray-200 dark:border-gray-700">{label}</div>
      <pre className="p-3 m-0 text-sm leading-6 font-mono overflow-x-auto bg-white dark:bg-gray-900">{children}</pre>
    </div>
  );
}

/* Renders lines with trailing comments aligned on a common column */
function Lines({ lines }: { lines: { text: number, node: ReactNode, comment?: string }[] }) {
  const width = Math.max(...lines.filter(l => l.comment).map(l => l.text), 0);
  return (
    <>
      {lines.map((l, i) => (
        <div key={i}>
          {l.node}
          {l.comment && <span className={C.comment}>{' '.repeat(width - l.text + 2)}// {l.comment}</span>}
        </div>
      ))}
    </>
  );
}

function StructShape({ fields }: { fields: Field[] }) {
  const lines = fields.map((f, i) => {
    const comma = i < fields.length - 1 ? ',' : '';
    const key = `"${f.name}"${f.optional ? '?' : ''}`;
    return {
      text: 2 + key.length + 2 + exprText(f.type).length + comma.length,
      comment: f.description,
      node: (
        <>
          {'  '}<span className={C.key}>{key}</span><span className={C.punct}>: </span><Expr expr={f.type}/><span className={C.punct}>{comma}</span>
        </>
      ),
    };
  });
  return (
    <>
      <div className={C.punct}>{'{'}</div>
      <Lines lines={lines}/>
      <div className={C.punct}>{'}'}</div>
    </>
  );
}

function EnumShape({ values }: { values: { value: string, ref?: string, description?: string }[] }) {
  const lines = values.map((v, i) => {
    const prefix = i === 0 ? '  ' : '| ';
    const text = JSON.stringify(v.value);
    return {
      text: prefix.length + text.length,
      node: <><span className={C.punct}>{prefix}</span><span className={C.string}>{text}</span></>,
      comment: v.description,
      ref: v.ref,
    };
  });
  const width = Math.max(...lines.map(l => l.text));
  return (
    <>
      {lines.map((l, i) => (
        <div key={i}>
          {l.node}
          {l.ref && <span className={C.comment}>{' '.repeat(width - l.text + 2)}// → <TypeRef name={l.ref}/></span>}
          {!l.ref && l.comment && <span className={C.comment}>{' '.repeat(width - l.text + 2)}// {l.comment}</span>}
        </div>
      ))}
    </>
  );
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
        {value.map((v, i) => <>{pad}{'  '}<JsonValue value={v} indent={indent + 1}/>{i < value.length - 1 && <span className={C.punct}>,</span>}{'\n'}</>)}
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
        <>{pad}{'  '}<span className={C.key}>{JSON.stringify(k)}</span><span className={C.punct}>: </span><JsonValue value={v} indent={indent + 1}/>{i < entries.length - 1 && <span className={C.punct}>,</span>}{'\n'}</>
      ))}
      {pad}<span className={C.punct}>{'}'}</span>
    </>
  );
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
        <span className={`text-xs font-medium px-2 py-0.5 rounded-full ${KIND_STYLES[def.kind]}`}>{def.kind}</span>
        <a href={`#${anchor}`} className="text-gray-400 opacity-0 group-hover:opacity-100 hover:text-blue-500" aria-label={`Link to ${def.name}`}>#</a>
      </header>
      {def.description && <div className="mb-3 text-gray-700 dark:text-gray-300 space-y-2">{def.description}</div>}
      {def.kind === 'union' && def.discriminator && (
        <p className="mb-3 text-sm text-gray-600 dark:text-gray-400">
          Discriminated by the <code className="font-mono">{def.discriminator}</code> field.
        </p>
      )}
      <div className={`grid gap-3 ${def.example !== undefined ? 'md:grid-cols-2' : ''}`}>
        <CodeBlock label="Shape">
          {def.kind === 'struct' && <StructShape fields={def.fields}/>}
          {def.kind === 'enum' && <EnumShape values={def.values}/>}
          {def.kind === 'union' && <UnionShape of={def.of}/>}
        </CodeBlock>
        {def.example !== undefined && (
          <CodeBlock label="Example">
            <JsonValue value={def.example} indent={0}/>
          </CodeBlock>
        )}
      </div>
    </section>
  );
}

export function TypeIndex({ defs }: { defs: TypeDef[] }) {
  return (
    <nav className="mb-8 flex flex-wrap gap-2">
      {defs.map(d => (
        <a key={d.name} href={`#${typeAnchor(d.name)}`} className="font-mono text-sm px-2 py-1 rounded border border-gray-200 dark:border-gray-700 hover:bg-gray-100 dark:hover:bg-gray-800">
          {d.name}
        </a>
      ))}
    </nav>
  );
}
