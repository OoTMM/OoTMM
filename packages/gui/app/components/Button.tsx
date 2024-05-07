import React, { forwardRef } from 'react';
import { Button as ButtonComponent } from 'react-bootstrap';

interface ButtonProps {
  ariaLabel?: string;
  border?: string;
  className?: string;
  disabled?: boolean;
  handleClick?: (...args: unknown[]) => unknown;
  iconCenter?: React.ReactElement;
  iconLeft?: React.ReactElement;
  iconRight?: React.ReactElement;
  label: string;
  onMouseLeave?: (...args: unknown[]) => unknown;
  onMouseOver?: (...args: unknown[]) => unknown;
  type?: 'button' | 'submit' | 'reset' | undefined;
  variant?: 'primary' | 'danger' | 'warning';
}

/**
 * Interactable Button component
 * * See https://react-bootstrap.github.io/components/buttons/ for examples and documentation

 * @param {Object} props - Object with all of the properties used within the react component, listed below.
 * @property {String} [label=''] - Label shown on the button
 * @property {String} [type] - Indicate what the button should do
 * @property {Element} [iconCenter] - Provides icon for button in the center
 * @property {Element} [iconRight] - Provides icon for button on the right
 * @property {Element} [iconLeft] - Provides icon for button on the left
 * @property {String} [ariaLabel=label] - Replaces what is read to the screenreader
 * @property {String} [variant='primary'] - Changes the design style to one of our presets
 * @property {Function} handleClick - Function to call when the button is clicked
 * @property {String} [className=''] - Manually change the styling of the button by passing in a className.  Allows the use of custom CSS.
 * @property {Boolean} [disabled] - If true, button is disabled
 * @property {Function} [onMouseOver] - - Function enacted when the user moves their mouse off of the button.  Is usually used for svg style switches on buttons.
 * @property {Function} [onMouseLeave] - Function enacted when the user moves their mouse off of the button.  Is usually used for svg style switches on buttons.
 * @returns {JSX} Button Component
 */

const Button = forwardRef<HTMLButtonElement, ButtonProps>((props, ref) => {
  const { label, type, iconCenter, iconRight, iconLeft, ariaLabel, handleClick, variant, className, disabled, onMouseOver, onMouseLeave } =
    props;

  let buttonClass = className ? ` ${className}` : 'btn-fill';

  return (
    <ButtonComponent
      ref={ref}
      type={type}
      aria-label={ariaLabel || label}
      className={buttonClass}
      onClick={handleClick}
      disabled={disabled}
      onMouseOver={onMouseOver}
      onMouseLeave={onMouseLeave}
      variant={variant}
    >
      {iconLeft && <span className="iconLeft">{iconLeft}</span>}
      {iconCenter}
      {label}
      {iconRight && <span className="iconRight">{iconRight}</span>}
    </ButtonComponent>
  );
});

Button.displayName = 'Button';

Button.defaultProps = {
  variant: 'primary',
  className: '',
  label: '',
};

export default Button;
