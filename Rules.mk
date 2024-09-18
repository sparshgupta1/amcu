# List non-default rules

TARGET_DEPS=$(patsubst %.o,.%.d,$(filter %.o,$(TARGET_OBJ)))

.PHONY: help clean

# Define the dependency, but let each project Makefile define their
# own recipe of how to build the target file(s).
#
$(TARGET) : $(TARGET_OBJ)

ifneq (,$(findstring clean,$(MAKECMDGOALS)))
else ifeq ($(MAKECMDGOALS),help)
else
-include $(TARGET_DEPS)
endif

.%.d : %.c
	@set -e; rm -f $@; \
	$(CC) -pipe -MM -MP $(CPPFLAGS) $< > $@.$$$$; \
	sed -e 's/\($*\).o:/\1.o $@ :/g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.%.d : %.cpp
	@set -e; rm -f $@; \
	$(CC) -pipe -MM -MP $(CXXFLAGS) $< > $@.$$$$; \
	sed -e 's/\($*\).o:/\1.o $@ :/g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.%.d: %.S
	@set -e; rm -f $@; \
	$(CC) -pipe -MM -MP $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.so : $(TARGET_OBJ)
	@echo "   [SO] $@"
	$(Q)$(CC) $(LDFLAGS) -shared -o $@ $<

%.dis : %.o
	@echo "    [D] $@"
	$(Q)$(OBJDUMP) -d -S -h -t $< > $@

%.a : $(TARGET_OBJ)
	@echo "    [A] $@"
	$(Q)$(AR) -rc $@ $<

%.E : %.c
	@echo "    [E] $@"
	$(Q)$(CC) $(CPPFLAGS) -E -o $@ $<

%.E : %.cpp
	@echo "    [E] $@"
	$(Q)$(CC) $(CPPFLAGS) -E -o $@ $<

%.E : %.S
	@echo "    [E] $@"
	$(Q)$(CC) $(CPPFLAGS) -E -o $@ $<

%.o : %.S
	@echo "    [C] $@"
	$(Q)$(CC) -D__ASSEMBLER__ -x assembler-with-cpp $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o : %.c
	@echo "    [C] $@"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o : %.cpp
	@echo "    [CXX] $@"
	$(Q)$(CC) $(CPPFLAGS) -D__FNAME__=\"$(notdir $<)\" -c $< -o $@

%.s : %.c
	@echo "    [S] $@"
	$(Q)$(CC) $(CPPFLAGS) $(CFLAGS) -S $< -o $@

%.s : %.S
	@echo "    [S] $@"
	$(Q)$(CC) -D__ASSEMBLER__ -x assembler-with-cpp $(CPPFLAGS) $(CFLAGS) -S $< > $@

%.ldo : %.ld
	@echo "    [P] $@"
	$(Q)$(CPP) -D__ASSEMBLER__ -D__LINKER__ $(CPPFLAGS) -P -x assembler-with-cpp $< $@_temp; \
	sed -e 's/^$$.*//g' $@_temp > $@ ; \
	$(RM) $@_temp

$(TARGET_OBJ): $(ROOT_DIR)/include/fw_config.h


$(ROOT_DIR)/include/fw_config.h: $(CONFIG_FILE)
	$(RM) $@
	@echo "    [V] $(notdir $@)"
	@echo "/* This is  a generated file!" > $@
	@echo " * Edit the Config file and then run make to change this file." >> $@
	@echo " */" >> $@
	@echo "#ifndef __FW_CONFIG_H__" >> $@
	@echo "#define __FW_CONFIG_H__" >> $@
	@echo "" >> $@
	sed -r -f $(ROOT_DIR)/scripts/config-header.sed $< >> $@
	@echo "" >> $@
	@echo "#endif /* __FW_CONFIG_H__ */" >> $@

$(ROOT_DIR)/db/mc-help.db: $(ROOT_DIR)/db/mc-help-schema.sql $(ROOT_DIR)/db/mc-help-data.sql
	@echo "    [DB] $@"
	rm -f $@;
	$(ROOT_DIR)/db/create-db $@ $^

$(ROOT_DIR)/db/obj-def.db: $(ROOT_DIR)/db/obj-def-schema.sql $(ROOT_DIR)/db/obj-def-data.sql
	@echo "    [DB] $@"
	rm -f $@;
	$(ROOT_DIR)/db/create-db $@ $^

$(ROOT_DIR)/db/mc-calcmn.db: $(ROOT_DIR)/db/mc-calcmn-schema.sql $(ROOT_DIR)/db/mc-calcmn-data.sql
	python $(ROOT_DIR)/db/mc-calcmn-verify.py  $(ROOT_DIR)/db/mc-calcmn-data.sql
	@echo "    [DB] $@"
	rm -f $@;
	$(ROOT_DIR)/db/create-db $@ $^

SQLFLAGS="-DSQLITE_ENABLE_MEMOERY_MANAGEMENT=1 \
          -DSQLITE_OMIT_LOAD_EXTENSION=1 \
          -DSQLITE_STMTJRNL_SPILL=0 \
          -DSQLITE_OMIT_ALTERTABLE=1"

sqlite3.o : sqlite3.c
	@echo "    [C] $@ $(SQLFLAGS)"
	$(Q)$(CC) $(SQLFLAGS) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@echo "----| Cleaning $(notdir $(shell pwd))"

	$(Q)-$(RM) -r $(TARGET) $(TARGET_OBJ) $(TARGET_CPP_OBJ) $(TARGET_DEPS) $(TARGET_DEPS:.%.d=%.o) $(FILES_TO_CLEAN) fw_config.h *.map *.dis *.E *~  .*.d.* $(ROOT_DIR)/db/obj-def.db $(ROOT_DIR)/include/ObjDef.h ObjDef.cpp $(ROOT_DIR)/db/__pycache__/ $(ROOT_DIR)/db/mc-calcmn.db $(ROOT_DIR)/db/mc-help.db $(ROOT_DIR)/db/mc-calcmn.bin

cleanall: clean
	$(Q)-$(RM) -r sqlite3.o  McActor.o
