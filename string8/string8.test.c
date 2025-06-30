void string_copy()
{
  String8 destination = {};
  String8 source = DSE_STR8("hello world");

  dse_string_copy(source.text, &destination);

  assertion(dse_strings_are_equal(destination, source), "Got |%s|", destination.text);
}

// to c string

void test_string_format()
{
  String8 format = DSE_STR8("A %s without %s is just a %s.");
  String8 a = DSE_STR8("world");
  String8 b = DSE_STR8("L");
  String8 c = DSE_STR8("word");
  String8 result = dse_string_format(format, a, b, c);
  String8 target = DSE_STR8("A world without L is just a word.");
  bool are_equal = dse_strings_are_equal(result, target);
  assertion(are_equal, "(String Format) Expected %s, Got %s", target.text, result.text);
}

void test_string_printf()
{
  String8 format = DSE_STR8("A %s without %s is just a %s.");
  String8 a = DSE_STR8("world");
  String8 b = DSE_STR8("L");
  String8 c = DSE_STR8("word");
  // dse_string_printf(format, a, b, c);
}