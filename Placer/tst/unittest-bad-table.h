PLACER_SCHEMA(UnitTestBadTable)
PLACER_INTEGER(id)		PLACER_FIELD(PRIMARY KEY)
PLACER_TEXT16(name,64)		PLACER_FIELD()
PLACER_FLOAT(age)		PLACER_FIELD()
PLACER_BLOB(image,256)		PLACER_FIELD()
PLACER_INTEGER64(sn)		PLACER_FIELD()
PLACER_TEXT(ssn,10)		PLACER_FINAL()
PLACER_END()
