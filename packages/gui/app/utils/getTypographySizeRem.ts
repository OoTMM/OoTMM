import { TypographySize } from "../enums/TypographySize";

// Define typography styles for each size
const typography: Record<TypographySize, { rem: string }> = {
    [TypographySize.XS]: { rem: "0.7333334rem" },
    [TypographySize.SM]: { rem: "0.8666667rem" },
    [TypographySize.BASE]: { rem: "1rem" },
    [TypographySize.MD]: { rem: "1rem" },
    [TypographySize.LG]: { rem: "1.2rem" },
    [TypographySize.XL]: { rem: "1.6rem" },
    [TypographySize.XXL]: { rem: "2rem" },
    [TypographySize.XXXL]: { rem: "2.4rem" },
    [TypographySize.JB]: { rem: "3.2rem" },
    [TypographySize.MG]: { rem: "4rem" },
  };
  
// Utility function to get the rem value for a given typography size
export const getTypographySizeRem =(size: string): string => {
    const selectedSize = typography[size as TypographySize] || typography[TypographySize.BASE];
    return selectedSize.rem;
  }