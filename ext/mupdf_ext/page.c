#include "mupdf_ext.h"
#include "mupdf/fitz.h"


static VALUE rb_Page   = Qnil;
static VALUE rb_Doc   = Qnil;



void
page_free( VALUE rb_page ) {
    Page *page;
    Data_Get_Struct( rb_page, Page, page );

    printf("GC Free Page: %016" PRIxPTR " %016" PRIxPTR "\n", (uintptr_t)page->document, (uintptr_t)page );
    fz_free_page(page->document->fz_document, page->fz_page);

    free(page);

}

VALUE
page_number(VALUE self){
    Page *page;
    Data_Get_Struct( self, Page, page );
    return INT2FIX(page->number);
}

VALUE
page_create(Document *doc, int page_number){
    Page *page;
    page = malloc(sizeof(Page));
    page->number   = page_number;
    page->document = doc;
    fz_try(FZ_CONTEXT)
    {
        page->fz_page  = fz_load_page( doc->fz_document, page->number );
    }
    fz_catch(FZ_CONTEXT)
    {
        rb_raise(rb_eArgError, "Unable to open page");
    }
    // N.B. We do not rely on Ruby to free the page struct, instead it's freed along with the document.
    // We do it this way so the document doesn't bother re-allocating a new page if one is re-requested
    // and it's impossible for a page to be freed prior to the document
    return Data_Wrap_Struct( rb_Page, NULL, NULL, page );
}


VALUE page_save_file(VALUE rb_page, VALUE filename, VALUE rb_width){
    Page *page;
    VALUE rb_file_str;
    int desired_width = FIX2INT(rb_width);
    Data_Get_Struct( rb_page, Page, page );

    fz_matrix transform;
    fz_rect bounds;
    fz_bound_page(page->document->fz_document, page->fz_page, &bounds);

    int default_page_height = bounds.y1-bounds.y0,
        default_page_width  = bounds.x1-bounds.x0;

    printf("Bounds: %f x %f - %f x %f\n", bounds.x0, bounds.y0, bounds.x1, bounds.y1);

    printf("Default Size: %d x %d\n", default_page_width, default_page_height);

    int new_height = ( desired_width / (float)default_page_width  ) * default_page_height;

    printf("New Height: %d\n",new_height);


    /* printf("Scale %f x %f\n", scalex, scaley); */

    bounds.x0=0; bounds.y0=0;
    bounds.x1 = desired_width; bounds.y1 = new_height;

    printf("Bounds: %f x %f - %f x %f\n", bounds.x0, bounds.y0, bounds.x1, bounds.y1);


    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);
    fz_pixmap *pix = fz_new_pixmap_with_bbox(FZ_CONTEXT, fz_device_rgb(FZ_CONTEXT), &bbox);
    fz_clear_pixmap_with_value(FZ_CONTEXT, pix, 0xff);

    // Create a draw device with the pixmap as its target.
    // Run the page with the transform.

    fz_device *dev = fz_new_draw_device(FZ_CONTEXT, pix);
    fz_run_page(page->document->fz_document, page->fz_page, dev, &transform, NULL);
    fz_free_device(dev);
    rb_file_str = rb_funcall(filename, to_s, 0);

    fz_write_png(FZ_CONTEXT, pix, StringValuePtr(rb_file_str), 0);

    return rb_page;
}

/*
 * A Page is a page on a document
 */
void
Init_Page(VALUE doc){

    rb_Doc = doc;
    /* Document-module: MuPDF
     * this is the Foo module under Kernel
     */
    VALUE rb_mupdf_module = rb_const_get(rb_cObject, rb_intern("MuPDF"));


    rb_Page  = rb_define_class_under(rb_mupdf_module, "Page", rb_cObject);

    rb_define_method(rb_Page, "number", page_number,    0 );
    rb_define_method(rb_Page, "save",   page_save_file, 2 );

}
