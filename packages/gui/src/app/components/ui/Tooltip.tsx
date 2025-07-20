import clsx from 'clsx';
import { ComponentChildren } from 'preact';
import { useState, useId, useRef, useEffect, useLayoutEffect } from 'preact/hooks';
import { createPortal, ReactNode, RefObject } from 'preact/compat';
import { FaCircleQuestion } from 'react-icons/fa6';

type TooltipContentProps = {
  targetRef: RefObject<HTMLElement>;
  children?: ReactNode;
};
function TooltipContent({ targetRef, children }: TooltipContentProps) {
  const tooltipRef = useRef<HTMLDivElement>(null);
  const [style, setStyle] = useState<React.CSSProperties>({display: 'none'});

  useLayoutEffect(() => {
    const target = targetRef.current;
    const tooltip = tooltipRef.current;
    if (!target || !tooltip) return;

    const root = target.getRootNode() as Document | ShadowRoot;
    const rootElement = root instanceof Document ? root.body : root.host;

    tooltip.style.display = 'block';
    tooltip.style.visibility = 'hidden';

    const targetRect = target.getBoundingClientRect();
    const tooltipRect = tooltip.getBoundingClientRect();
    const containerRect = rootElement.getBoundingClientRect();

    const spacing = 6;
    const padding = 8;

    let top = targetRect.bottom + spacing + window.scrollY;
    let left = targetRect.left + targetRect.width / 2 + window.scrollX;
    let transform = 'translateX(-50%)';

    // Flip vertically if tooltip overflows the container bottom
    if (top + tooltipRect.height > containerRect.bottom) {
      top = targetRect.top - tooltipRect.height - spacing + window.scrollY;
    }

    // Clamp horizontally within container
    const leftEdge = left - tooltipRect.width / 2;
    const rightEdge = left + tooltipRect.width / 2;
    const containerLeft = containerRect.left + window.scrollX;
    const containerRight = containerRect.right + window.scrollX;

    if (leftEdge < containerLeft + padding) {
      left = containerLeft + padding + tooltipRect.width / 2;
    } else if (rightEdge > containerRight - padding) {
      left = containerRight - padding - tooltipRect.width / 2;
    }

    setStyle({
      top: `${top}px`,
      left: `${left}px`,
      transform,
      visibility: 'visible',
      position: 'absolute',
    });
  }, [targetRef.current]);

  return (
    <div ref={tooltipRef} style={style} className="absolute z-50 bg-neutral-900 p-4 text-white rounded pointer-events-none max-w-[768px]">
      {children}
    </div>
  );
}

type TooltipProps = {
  children: ComponentChildren;
};
export function Tooltip({ children }: TooltipProps) {
  const ref = useRef<HTMLSpanElement>(null);
  const id = useId();
  const [container, setContainer] = useState<HTMLElement | ShadowRoot | null>(null);
  const [shown, setShown] = useState(false);

  useEffect(() => {
    if (!ref.current) return;
    const root = ref.current.getRootNode() as Document | ShadowRoot;
    const body = root instanceof Document ? root.body : root;
    setContainer(body);
  }, []);

  return <>
    <span ref={ref} data-tooltip-id={id} onMouseEnter={() => setShown(true)} onMouseLeave={() => setShown(false)}><FaCircleQuestion/></span>
    {container && shown && createPortal(<TooltipContent targetRef={ref}>{children}</TooltipContent>, container)}
  </>;
}
