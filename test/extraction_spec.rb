require_relative 'spec_helper'
require 'tempfile'

describe MuPDF do

    let (:doc){ MuPDF::Document.open( pdf_path('basicapi') ) }

    it "can open a pdf file" do
        assert doc
    end

    it "counts the number of pages" do
        assert_equal 3, doc.page_count
    end

    it "opens a page" do
        assert_instance_of MuPDF::Page, doc.page_at(2)
        page = doc.page_at(2)
        assert_same page, doc.page_at(2)
        assert_instance_of MuPDF::Page, doc.page_at(2)
    end

    it "can iterate through pages" do
        i = 0
        doc.each_page do | page |
            assert_equal i, page.number
            i+=1
        end
    end

    it "can extract a page" do
        page = doc.page_at(1)
        tf = Tempfile.new(["test",".png"])
        page.save(tf.path, 800)
        assert_match "PNG 800x1130", `identify #{tf.path}`
    end
end
