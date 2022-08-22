SECTIONS
{
  . = 0x06000000;
  .cobject.data : { *(.cobject.data*); }

  . = 0xff000000;
  .cobject.name : { *(.cobject.name*); }
  .cobject.exports_addr : { *(.cobject.exports_addr*); }
  .cobject.exports_name : { *(.cobject.exports_name*); }

  /DISCARD/ : { *(.*); }
}
