export function IconTriforce({ size = 16 }: { size?: number | string }) {
  return (
    <svg
      viewBox="0 0 100 87"
      width={size}
      height={size}
      fill="currentColor"
    >
      <path d="M50 0 L75 43.3 L25 43.3 Z" />
      <path d="M25 43.3 L50 86.6 L0 86.6 Z" />
      <path d="M75 43.3 L100 86.6 L50 86.6 Z" />
    </svg>
  );
}
