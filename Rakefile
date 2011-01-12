
########
#
# Things you should know
#
# 
# Sometime when compiling with rake you get a stupid error saying it doesn't know how to build some include tasks... delete the ".depends.mf" file to
# get rid of this error.
#
# dummy-sear/dummy-sear-impl.h contains the broadcast address, the place where multicasts are sent to, this must correspond to the local network so
# the proper address should be in this file! 
#
# To manually compile C code for the gumstix use
# arm-linux-gcc  -I/usr/lib/realtime/include/ -I./rte/ -I./dummy-sear/ -I./memory -lm app/acceleration.c rte/*.c dummy-sear/*.c memory/*.c -lpthread -o OUTPUTNAME
#######
require 'rake/clean'
require 'rake/loaders/makefile'

#task :default => ['publish', 'subscribe', 'circle-publish', 'local']

#task :default => ['publish', 'subscribe', 'circle-publish']

task :default => ['main2']
#CC="gcc -g"
#CPP="g++ -g"
CC="/home/sean/gumstix/gumstix-buildroot/build_arm_nofpu/staging_dir/bin/arm-linux-gcc "
CPP="/home/sean/gumstix/gumstix-buildroot/build_arm_nofpu/staging_dir/bin/arm-linux-g++ "
SRC = FileList["rte/*.c", "base64.c" ,"dummy-sear/*.c", "memory/*.c","libmeh.c","steam_interface.c"]

MAIN2_SRC =SRC + ["example2.c"]
MAIN2_OBJ = MAIN2_SRC.ext('o')

MAIN3_SRC =SRC + ["example3.c"]
MAIN3_OBJ = MAIN3_SRC.ext('o')


MAIN4_SRC =SRC + ["example4.c"]
MAIN4_OBJ = MAIN4_SRC.ext('o')

MAIN5_SRC =SRC + ["example5.c"]
MAIN5_OBJ = MAIN5_SRC.ext('o')

MAIN6_SRC =SRC + ["example6.c"]
MAIN6_OBJ = MAIN6_SRC.ext('o')

MAIN7_SRC =SRC + ["example7.c"]
MAIN7_OBJ = MAIN7_SRC.ext('o')

EVENT_SRC =SRC + ["event_source.c"]
EVENT_SRC_OBJ = EVENT_SRC.ext('o')

EVENT_ACC =SRC + ["accel.c"]
EVENT_ACC_OBJ = EVENT_ACC.ext('o')

EVENT_ORIENT =SRC + ["orient.c"]
EVENT_ORIENT_OBJ = EVENT_ORIENT.ext('o')

EVENT_SINK_SRC =SRC + ["event_sink.c"]
EVENT_SINK_OBJ = EVENT_SINK_SRC.ext('o')

SWING_START_SRC =SRC + ["swing_started.c"]
SWING_START_OBJ = SWING_START_SRC.ext('o')

SWING_END_SRC =SRC + ["swing_end.c"]
SWING_END_OBJ = SWING_END_SRC.ext('o')
BALL_CONTACT_SRC =SRC + ["ball_contact.c"]
BALL_CONTACT_OBJ = BALL_CONTACT_SRC.ext('o')

ORIENT_ON_CONTACT_SRC =SRC + ["orient_on_contact.c"]
ORIENT_ON_CONTACT_OBJ = ORIENT_ON_CONTACT_SRC.ext('o')

STANCE_ON_CONTACT_SRC = SRC + ["stance_on_contact.c"]
STANCE_ON_CONTACT_OBJ = STANCE_ON_CONTACT_SRC.ext('o')


FALL_DETECTION_SRC = SRC + ["fall_detection.c"]
FALL_DETECTION_OBJ = FALL_DETECTION_SRC.ext('o')

LIBB64_OBJ = ["./libb64/src/b64enc.o"]


CFLAGS = "-I/usr/lib/realtime/include/ -I./rte/ -I./dummy-sear/ -I./memory/"
LDFLAGS = "-lpthread -lm"
OBJ = MAIN2_OBJ + MAIN3_OBJ + MAIN4_OBJ + MAIN5_OBJ+  EVENT_SRC_OBJ + LIBB64_OBJ + MAIN6_OBJ + MAIN7_OBJ + EVENT_SINK_OBJ + EVENT_ORIENT_OBJ + EVENT_ACC_OBJ + SWING_START_OBJ + SWING_END_OBJ
CLEAN.include(OBJ)
CLOBBER.include(['publish', 'subscribe', 'circle-publish','local','local-consumer','main','main2','main3','main4','event_source','event_sink','accel','orient', 'swing_start', 'swing_end', 'stance_on_contact', 'ball_contact', 'orient_on_contact','fall_detection'])
CLOBBER.include(OBJ)

file ".depends.mf" => SRC do |t|
  sh "makedepend -f- -- #{CFLAGS} -- #{t.prerequisites} > #{t.name}" 
end

import ".depends.mf" 
rule '.o' => '.c' do |t|
 sh "#{CC} #{CFLAGS} -c -o #{t.name} #{t.source}"
end


file "main2" => MAIN2_OBJ do
  sh "#{CC} #{LDFLAGS} -o main2 #{MAIN2_OBJ}"
end


file "main3" => MAIN3_OBJ do
  sh "#{CC} #{LDFLAGS} -o main3 #{MAIN3_OBJ}"
end


file "main4" => MAIN4_OBJ do
  sh "#{CC} #{LDFLAGS} -o main4 #{MAIN4_OBJ}"
end

file "main5" => MAIN5_OBJ do
  sh "#{CC} #{LDFLAGS} -o main5 #{MAIN5_OBJ}"
end

file "main6" => MAIN6_OBJ do
  sh "#{CC} #{LDFLAGS} -o main6 #{MAIN6_OBJ}"
end

file "main7" => MAIN7_OBJ do
  sh "#{CC} #{LDFLAGS} -o main7 #{MAIN7_OBJ}"
end

file "event_sink" => EVENT_SINK_OBJ do
  sh "#{CC} #{LDFLAGS} -o event_sink #{EVENT_SINK_OBJ}"
end


file "event_source" => EVENT_SRC_OBJ do
  sh "#{CC} #{LDFLAGS} -o event_source #{EVENT_SRC_OBJ}"
end

file "accel" => EVENT_ACC_OBJ do
  sh "#{CC} #{LDFLAGS} -o accel #{EVENT_ACC_OBJ}"
end


file "orient" => EVENT_ORIENT_OBJ do
  sh "#{CC} #{LDFLAGS} -o orient #{EVENT_ORIENT_OBJ}"
end


file "swing_start" => SWING_START_OBJ do
  sh "#{CC} #{LDFLAGS} -o swing_start #{SWING_START_OBJ}"
end


file "swing_end" => SWING_END_OBJ do
  sh "#{CC} #{LDFLAGS} -o swing_end #{SWING_END_OBJ}"
end


file "ball_contact" => BALL_CONTACT_OBJ do
  sh "#{CC} #{LDFLAGS} -o ball_contact #{BALL_CONTACT_OBJ}"
end

file "orient_on_contact" => ORIENT_ON_CONTACT_OBJ do
  sh "#{CC} #{LDFLAGS} -o orient_on_contact  #{ORIENT_ON_CONTACT_OBJ}"
end


file "stance_on_contact" => STANCE_ON_CONTACT_OBJ do
  sh "#{CC} #{LDFLAGS} -o stance_on_contact  #{STANCE_ON_CONTACT_OBJ}"
end


file "fall_detection" => FALL_DETECTION_OBJ do
  sh "#{CC} #{LDFLAGS} -o fall_detection  #{FALL_DETECTION_OBJ}"
end
