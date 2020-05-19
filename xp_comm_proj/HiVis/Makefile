info:
	@echo "Please use the toplevel Makefile unless you know what you do!"

doc:
	kdoc -H -dVisApi/doc/api -p -q -a VisAPI VisApi/*.h VisApi/AVS/*.h

TARGET=/mnt/sgizenger1/NOSAVE/avs/HiVis


build:

distclean:
	rm -f `find . -name "*~" | xargs`
	rm -f `find . -name "*.pbi" | xargs`
	rm -f `find . -name "*.pbt" | xargs`
	rm -f `find . -name "*.o" | xargs`
	rm -f `find . -name "*.obj" | xargs`
	rm -rf `find . -name "ii_files" | xargs`

clean:
