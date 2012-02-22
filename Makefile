.SUFFIXES: .dot .gif .bmp

.dot.gif:
	dot -Tgif $< -o $@

.gif.bmp:
	convert $< `basename $< .gif`.bmp

rbt_000.dot: rbt_gen
	./rbt_gen

rbt_000.gif: rbt_000.dot
	for f in rbt_*.dot ; do dot -Tgif "$$f" -o `basename $$f .dot`.gif ; done

rbt_anim.gif: rbt_000.gif topcenter
	awk -f rbt_anim_gen.awk

rbt_gen: rbt_gen.cc
	g++ -O2 -o $@ $<

topcenter: topcenter.c
	gcc -O2 -o $@ $<

clean:
	rm -f rbt_*.dot rbt_*.gif rbt_gen topcenter *~
