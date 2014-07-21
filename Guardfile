guard :minitest do
    watch(%r{^test/(.*)_spec\.rb$})
end

guard 'rake', :task => 'buildtest' do
    watch %r{ext/mupdf_ext/.*\.(c|h)$}
end

# guard :shell do
#   watch /ext\/.*\.c/ do |m|
#     if system("cd ext; make")

#     end
#   end
# end
