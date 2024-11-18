const env = process.env.NODE_ENV || 'development';
const isProd = (env === 'production');

let STATIC_URL: string;

if (isProd) {
  STATIC_URL = 'https://static.ootmm.com';
} else {
  STATIC_URL = '';
}

export { STATIC_URL };
