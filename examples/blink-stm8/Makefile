.PHONY: clean flash

OBJECTS=main.rel fsmosc.rel

main.ihx: $(OBJECTS)
	sdcc -mstm8 --std-c99 --Werror $^ -o $@

fsmosc.rel: ../../src/fsmosc.c
	sdcc -c -mstm8 --std-c99 --Werror -I../../src $< -o $@

%.rel: %.c
	sdcc -c -mstm8 --std-c99 --Werror -I../../src $< -o $@

flash: main.ihx
	stm8flash -c stlinkv2 -p stm8s103f3 -w $<

clean:
	rm -f *.asm *.cdb *.ihx *.lk *.lst *.map *.rel *.rst *.sym
