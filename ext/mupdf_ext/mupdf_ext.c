/*
 * mupdf_ext.c - Toplevel extension
 * $Id$
 *
 * Author: Nathan Stitt <nathan@stitt.org>
 *
 * You may redistribute this software under the terms of the
 * Affero General Public License.
 * http://www.gnu.org/licenses/Apple-3.0.html or the LICENSE-AGPL.txt
 * file in the source for details.
 */

#include "mupdf_ext.h"
#include <inttypes.h>


/*
MuPDF Module

Holds the stuffs
*/
static VALUE rb_MuPDF = Qnil;

ID to_s;
fz_context *FZ_CONTEXT;


void
Init_mupdf_ext()
{
    to_s = rb_intern ("to_s");
    FZ_CONTEXT = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(FZ_CONTEXT);
    /*
      MuPDF Module

      Holds the stuffs
    */
    rb_MuPDF = rb_define_module( "MuPDF" );

    Init_Document();


}
