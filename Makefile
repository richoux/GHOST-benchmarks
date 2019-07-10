all:
	$(MAKE) build_order
	$(MAKE) resource
	$(MAKE) target
	$(MAKE) wallin

build_order:
	(cd build_order && $(MAKE))

resource:
	(cd resource && $(MAKE))

target:
	(cd target && $(MAKE))

wallin:
	(cd wallin && $(MAKE))

clean:
	(cd build_order && $(MAKE) clean)
	(cd resource && $(MAKE) clean)
	(cd target && $(MAKE) clean)
	(cd wallin && $(MAKE) clean)
