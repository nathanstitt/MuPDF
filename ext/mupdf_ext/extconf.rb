require 'pp'
require 'mkmf'

# Idea via the pg gem
if ENV['MAINTAINER_MODE']
    $stderr.puts "Maintainer mode enabled."
    $CFLAGS <<
    ' -Wall' <<
    ' -ggdb' <<
    ' -DDEBUG' <<
    ' -pedantic'
end

find_header( "mupdf/fitz.h" ) or abort "Can't find MuPDF mupdf/fitz.h header"
have_library("mupdf") or abort "Can't find MuPDF library libmupdf"

have_library( 'crypto', 'BIO_new' ) or abort "Can't find crypto library"
have_library( 'freetype', 'FT_Init_FreeType' ) or abort "Can't find FreeType library"

unless have_header("jpeglib.h") && have_header("jerror.h") &&
   (have_library("jpeg", "jpeg_set_defaults") ||
    have_library("libjpeg", "jpeg_set_defaults"))
   abort "Can't find libjpeg library"
end

if have_library("z", "deflate")
    have_library("png", "png_create_read_struct")
end

append_library( $libs, 'crypto' )


create_makefile( "mupdf_ext" )
