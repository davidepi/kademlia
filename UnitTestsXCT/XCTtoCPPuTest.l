%START FILENAME DELETE_EVERYTHING REQUEST_1NAME REQUEST_2NAME

%%
^"#import".+\n {}
^"@interface".+\n {}
^"@end".*\n {}
^"@implementation " {printf("%s\n","#include <gtest/gtest.h>");BEGIN FILENAME;printf("%s","TEST_GROUP(");}
<FILENAME>[a-zA-Z0-9][a-zA-Z0-9]+ {ECHO;printf("%s",")\n{ };");}
<FILENAME>_tests {BEGIN 0;}

^"- (void)setUp"\ +\{    {BEGIN DELETE_EVERYTHING;}
^"- (void)tearDown"\ +\{ {BEGIN DELETE_EVERYTHING;}
<DELETE_EVERYTHING>\}    {BEGIN 0;}
<DELETE_EVERYTHING>.     {}

^"-"\ *"(void)"\ ?"test"[0-9]+_ {BEGIN REQUEST_1NAME;printf("%s","TEST(");}
<REQUEST_1NAME>[a-zA-Z0-9]      {ECHO;}
<REQUEST_1NAME>_                {BEGIN REQUEST_2NAME;printf("%s",",");}
<REQUEST_2NAME>.+               {ECHO;printf("%s",")");BEGIN 0;}

"XCTAssertTrue"  {printf("EXPECT_TRUE");}
"XCTAssertFalse" {printf("EXPECT_FALSE");}
"XCTAssertEqual" {printf("EXPECT_EQ");}

^\ *\n+ {printf("\n");}
. {ECHO;}

%%

int main( int argc, char **argv )
{
	++argv, --argc;  /* skip over program name */
	if ( argc > 0 )
		yyin = fopen( argv[0], "r" );
	else
		yyin = stdin;
    yylex();
	return 0;
}
