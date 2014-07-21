#include "mupdf_ext.h"
#include "mupdf/fitz.h"


static VALUE rb_Doc   = Qnil;
ID rb_pages_ivar;


VALUE rb_mupdf_module;

static void
doc_gc_free( Document *doc ) {
    VALUE rb_page;
    printf("GC Free Document: %016" PRIxPTR "\n", (uintptr_t)doc );

    for ( int pg=0; pg < fz_count_pages(doc->fz_document); pg++ ){
        rb_page = doc->pages[pg];
        if (rb_page){
            page_free(rb_page);
        }
    }
    fz_close_document(doc->fz_document);
    free(doc->pages);
    free(doc);
}

static void
doc_gc_mark(Document *doc){
    VALUE page;
    printf("GC Mark Document: %016" PRIxPTR "\n", (uintptr_t)doc );
    for ( int pg=0; pg < fz_count_pages(doc->fz_document); pg++ ){
        page = doc->pages[pg];
        if ( page ){
            printf("GC Mark Document: %016" PRIxPTR "\n", (uintptr_t)doc );
            rb_gc_mark(page);
        }
    }
}

VALUE
doc_open(VALUE klass, VALUE path){
    Document *document;
    fz_document *doc;
    size_t pg_size;
    VALUE str = rb_funcall(path, to_s, 0);
    fz_try(FZ_CONTEXT)
    {
        doc = fz_open_document(FZ_CONTEXT, StringValuePtr(str));
    }
    fz_catch(FZ_CONTEXT)
    {
        rb_raise(rb_eArgError, "Unable to open file");
    }
    pg_size = sizeof(VALUE)*fz_count_pages(doc);
    document = malloc(sizeof(Document));
    printf("New Document: %016" PRIxPTR "\n", (uintptr_t)document );

    document->fz_document = doc;
    document->pages = malloc(pg_size);
    memset( document->pages, 0, pg_size); // important - set the pages to 0 so we can tell which are later allocated
    VALUE self = Data_Wrap_Struct( rb_Doc, &doc_gc_mark, &doc_gc_free, document );
    return self;
}


VALUE
doc_page_at(VALUE doc, VALUE pg){
    VALUE page;
    int  page_number;
    Document *document;

    Data_Get_Struct( doc, Document, document);
    page_number = FIX2INT(pg);
    if ( page_number < 0 || page_number > fz_count_pages(document->fz_document) ){
        rb_raise(rb_eArgError, "Page Number must be between 0..%d",page_number);
    }
    page = document->pages[page_number];
    if ( ! page ){
        page = page_create(document, page_number);
        rb_gc_mark(page);
        document->pages[page_number] = page;
    }
    printf("Doc page_at: %016" PRIxPTR " %lu\n", (uintptr_t)document, page);
    return page;
}

VALUE
doc_page_count(VALUE self){
    Document *doc;
    Data_Get_Struct(self,Document, doc);
    return INT2FIX( fz_count_pages(doc->fz_document) );
}

VALUE
doc_each_page(VALUE self){
    Document *doc;
    Data_Get_Struct(self, Document, doc);

    for( int page=0; page < fz_count_pages(doc->fz_document); page++){
        rb_yield( doc_page_at(self, INT2FIX(page)) );
    }
    return self;
}

void
Init_Document(){

    VALUE rb_mupdf_module = rb_const_get(rb_cObject, rb_intern("MuPDF"));

    rb_pages_ivar = rb_intern ("pages");

    /*
      MuPDF::Document class

      Holds the pdf and has pages
    */
    rb_Doc   = rb_define_class_under(rb_mupdf_module, "Document", rb_cObject);
    Init_Page(rb_Doc);
    rb_define_singleton_method(rb_Doc, "open", doc_open, 1);

    rb_define_method(rb_Doc,"each_page",doc_each_page,    0);
    rb_define_method(rb_Doc,"page_at", doc_page_at,    1 );

    rb_define_method(rb_Doc,"page_count",doc_page_count,  0);

}
