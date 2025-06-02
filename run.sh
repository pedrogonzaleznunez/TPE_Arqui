#!/bin/bash
# flags -audiodev coreaudio,id=speaker -machine pcspk-audiodev=speaker usadas para sonido
# flag -rtc base=localtime agregada para que se ajuste el horario
# en osDev se usa pa en -audiodev, pero no en macOs no se instala con el qemu

qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev coreaudio,id=speaker \
-machine pcspk-audiodev=speaker -rtc base=localtime
