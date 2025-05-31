#!/bin/bash
# flags -audiodev pa,id=speaker -machine pcspk-audiodev=speaker usadas para sonido
# flag -rtc base=localtime agregada para que se ajuste el horario
qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker -rtc base=localtime
