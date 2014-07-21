require "bundler/gem_tasks"
require 'rake/testtask'
require 'rdoc/task'

RDOC_FILES = FileList["README.md","ext/mupdf_ext/*.c"]
p RDOC_FILES
Rake::RDocTask.new do |rd|
    rd.main = "README.md"
    rd.rdoc_dir = "doc/site/api"
    rd.rdoc_files.include(RDOC_FILES)
end

Rake::RDocTask.new(:ri) do |rd|
    rd.main = "README.md"
    rd.rdoc_dir = "doc/ri"
    rd.options << "--ri-system"
    rd.rdoc_files.include(RDOC_FILES)
end


Rake::TestTask.new do |t|
    t.libs << 'test'
    t.pattern = "test/*_spec.rb"
end

require 'rake/extensiontask'
Rake::ExtensionTask.new('mupdf_ext')


task :buildtest => :compile do
    Rake::Task["test"].invoke
end
