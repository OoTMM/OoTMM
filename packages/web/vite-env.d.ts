declare const __STATIC_URL__: string;

declare module "*.mdx" {
  let MDXComponent: (props: any) => JSX.Element;
  export default MDXComponent;
}

declare module "*.png" {
  const src: string;
  export default src;
}
