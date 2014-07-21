# MuPDF

MuPDF is a partial set of bindings to the [MuPDF](http://www.mupdf.com) viewer's API.


## Installation

Add this line to your application's Gemfile:

    gem 'mupdf'

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install mupdf

## Usage

Save each page with the width set to 768.  A saner API for sizes is forthcoming

    document = MuPDF::Document.open( 'test/example.pdf' )
    document.each_page do |page|
        page.save( "test/page-#{page.number}.png", 768 )
    end


## Contributing

1. Fork it ( http://github.com/<my-github-username>/mupdf/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create new Pull Request
