# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'mupdf/version'

Gem::Specification.new do |spec|
  spec.name          = "mupdf"
  spec.version       = Mupdf::VERSION
  spec.authors       = ["Nathan Stitt"]
  spec.email         = ["nathan@stitt.org"]
  spec.summary       = %q{A set of bindings for the MuPDF library.}
  spec.description   = %q{A set of bindings for the MuPDF library.  MuPDF is a small, fast and complete implementation of the PDF 1.7.  It supports transparency, encryption, hyperlinks, annotations, and searching.}
  spec.homepage      = ""
  spec.license       = "AGPL-3.0"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.add_dependency "free-image", "~>0.6.2"

  spec.add_development_dependency "bundler", "~> 1.5"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "guard-minitest"
  spec.add_development_dependency "guard-rake"
  spec.add_development_dependency "rake-compiler"

end
