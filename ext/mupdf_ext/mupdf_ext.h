#ifndef _INCLUDE_MUPDF_EXT_H_
#define _INCLUDE_MUPDF_EXT_H_

#include "ruby.h"
#include "mupdf/fitz.h"

extern fz_context *FZ_CONTEXT;
extern ID to_s;

typedef struct _document {
    VALUE *pages;
    fz_document *fz_document;
} Document;


typedef struct _page {
    int number;
    Document *document;
    fz_page *fz_page;
} Page;


void Init_Document();
void Init_Page(VALUE doc_klass);

VALUE page_create(Document *document, int page_number);
void  page_free( VALUE page );

#endif // _INCLUDE_MUPDF_EXT_H_
