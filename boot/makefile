
.DEFAULT:=bootloader.img

bootloader.img: bootsect.bin bootloader.bin
	cat $^ | dd of=$@ bs=512

bootsect.bin: bootsect.asm
	nasm -f bin -o $@ $<

bootloader.bin: bootloader.asm a20.inc
	nasm -f bin -o $@ $<

clean:
	rm bootloader.img; rm bootloader.bin; rm bootsect.bin
