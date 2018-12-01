import junit.framework.TestCase;
import java.util.Random;
import java.security.SecureRandom;
//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!

public class UrlValidatorTest extends TestCase {

   public UrlValidatorTest(String testName) {
      super(testName);
   }
   
   String[] newSchemes = { "http", "https" };
   
   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES); //all schemes
   UrlValidator urlVal2 = new UrlValidator(newSchemes); //use new schemes
   UrlValidator urlVal3 = new UrlValidator(); // default schemes (http, https and ftp)

   String url; //stores a URL
   String valid = "Valid";
   String invalid = "Invalid";
   String error = "Error";
   
   String schemeAuthority = "http://google.com"; // we know this is true
   String validUrlNoQuery = "http://google.com/test1"; //we know this is true
   
   //outputs to screen results of isValid
   private void testUrl(String url, String expected) {
	   if(expected == "Error") {
		   System.out.println("ERROR: " + url);
	   }
	   else if (urlVal.isValid(url) == true)
	   	{
	   		System.out.println("Valid URL: " + url + "\nExpected: " + expected + "\n");	
	   	}
	   else {
		   System.out.println("Invalid URL: " + url  + "\nExpected: " + expected + "\n");	
	   }
   }
   
 //outputs to screen results of isValid -- extra argument
   private void testUrl(String url, String expected, UrlValidator urlVal) {
	   if(expected == "Error") {
		   System.out.println("ERROR: " + url + "\n");
	   }
	   else if (urlVal.isValid(url) == true)
	   	{
	   		System.out.println("Valid URL: " + url + "\nExpected: " + expected + "\n");	
	   	}
	   else {
		   System.out.println("Invalid URL: " + url  + "\nExpected: " + expected + "\n");	
	   }
   }
   
   public void testManualTest()
   {	   
	   
	   //System.out.println(urlVal.isValid("http://google.com"));
	   System.out.println("****** Manual Tests ******\n");
	   testUrl("http://google.com", valid, urlVal); // all
	   testUrl("http://google.com", valid, urlVal2); //passed in schemes
	   testUrl("http://google.com", valid, urlVal3); //default
	   System.out.println("");
	   
	   testUrl("https://google.com", error, urlVal); //causes error -- change the variable error to invalid/valid to see error
	   testUrl("https://google.com", valid, urlVal2);
	   testUrl("https://google.com", valid, urlVal3);
	   System.out.println("");
	   
	   testUrl("ftp://google.com", error, urlVal); // causes error -- change the variable error to invalid/valid to see error
	   testUrl("ftp://google.com", invalid, urlVal2); 
	   testUrl("ftp://google.com", valid, urlVal3); 
	   System.out.println("");
	   
	   testUrl("http://tech.yahoo.com/rc?hungry", valid, urlVal);
	   testUrl("http://tech.yahoo.com/rc?hungry", valid, urlVal2); 
	   testUrl("http://tech.yahoo.com/rc?hungry", valid, urlVal3); 
	   System.out.println("");
	   
	   testUrl("3ht://google.com/extra?action=view", invalid, urlVal); 
	   testUrl("3ht://google.com/extra?action=view", invalid, urlVal2);
	   testUrl("3ht://google.com/extra?action=view", invalid, urlVal3);
	   System.out.println("");
	   
	   testUrl("http://you.?//.14413.52332.52423.3234.23123.43.1.23.3...,<>%.tube.com/view?movies=narnia", invalid, urlVal); 
	   testUrl("http://you.?//.14413.52332.52423.3234.23123.43.1.23.3...,<>%.tube.com/view?movies=narnia", invalid, urlVal2);
	   testUrl("http://you.?//.14413.52332.52423.3234.23123.43.1.23.3...,<>%.tube.com/view?movies=narnia", invalid, urlVal3);
	   System.out.println("");
	   
   }

   public void testYourFirstPartition()
   {
	   //Schemes
	   
	   //SCHEME_REGEX = "^\\p{Alpha}[\\p{Alnum}\\+\\-\\.]*"
	   //                   1       2
	   // Matching from the beginning of the string, in order:
	   // $1 One alphabetical char a-z and A-Z only
	   // $2 Match 0 or more alphanumeric chars, "+", "-", or "."
	   System.out.println("\n****** Scheme ******");
	   testUrl("http://google.com", valid);
	   testUrl("3ht://google.com", invalid);
	   // According to RFC 2396, Appendix B the delimiter "//" is actually part of the authority
	   testUrl("http:/google.com", invalid);
	   testUrl("http:google.com", invalid);
	   testUrl("http/google.com", invalid);
	   testUrl("://google.com", invalid); 
	   testUrl("" + "google.com", valid); // Believe this is actually invalid
	   
	   //tests that cause errors -- change the variable error to invalid/valid to see error
	   testUrl("ftp://google.com", error); // ftp:// causes error -- should be valid
	   testUrl("h3t://google.com", error); // h3t:// causes error -- should be valid
   }
   
   public void testYourSecondPartition(){
	   
	   //Authority
	   //AUTHORITY_CHARS_REGEX = "\\p{Alnum}\\-\\."
	   //                         1
	   // 	$1 Match alphanumeric chars or "-" or "."
	   System.out.println("\n****** Authority ******");
	   testUrl("http://www.google.com", valid);
	   testUrl("http://go.com", valid);
	   testUrl("http://go.au", valid);
	   testUrl("http://0.0.0.0", valid);
	   testUrl("http://255.255.255.255", valid);
	   testUrl("http://256.256.256.256", invalid);
	   testUrl("http://255.com", valid);
	   testUrl("http://1.2.3.4.5", invalid);
	   testUrl("http://1.2.3.4.", invalid);
	   testUrl("http://1.2.3", invalid);
	   testUrl("http://.1.2.3.4", invalid);
	   testUrl("http://go.a", invalid);
	   testUrl("http://go.a1a", invalid);
	   testUrl("http://go.1aa", invalid);
	   testUrl("http://aaa.", invalid);
	   testUrl("http://.aaa", invalid);
	   testUrl("http://aaa", invalid);
	   testUrl("http://", invalid);
	   testUrl("file:", valid); // file: scheme allows empty authority
	   testUrl("file://localhost:", invalid); // file: scheme does not allow trailing ":"
	   testUrl("file://localhost", valid); // file: scheme allows authority
	   testUrl("http://127.0.0.1", valid); // Should allow localhost
	   testUrl("http://localhost", valid); // Should allow localhost
   }
   
   public void testYourThirdPartition(){
	   //Port
	   //UrlValidator should simply check to see if port is empty or contains only integers < MAX_UNSIGNED_SHORT
	   System.out.println("\n****** Port ******");
	   testUrl(schemeAuthority + ":80", valid);
	   testUrl(schemeAuthority + ":65535", valid); // Max port length = MAX_UNSIGNED_SHORT 0xffff = 65535
	   testUrl(schemeAuthority + ":0", valid);
	   testUrl(schemeAuthority + "", valid);
	   testUrl(schemeAuthority + ":-1", invalid);
	   testUrl(schemeAuthority + ":65636", invalid); // Max port length = MAX_UNSIGNED_SHORT 0xffff = 65535
	   testUrl(schemeAuthority + ":65a", invalid);
   }
   public void testYourFourthPartition(){
	   //testpath
	   //PATH_REGEX = "^(/[-\\w:@&?=+,.!/~*'%$_;\\(\\)]*)?$";
	   //               1                             
	   // 	Matching from the beginning of the string, in order:
	   // 	$1 Must start with "/" then match (zero or more times) any word or char in : @ & ? = + , . ! / ~ * ' % $ _ ; ( )
	   System.out.println("\n****** TestPath ******");
	   testUrl(schemeAuthority + "/test1", valid);
	   testUrl(schemeAuthority + "/t123", valid);
	   testUrl(schemeAuthority + "/$23", valid);
	   testUrl(schemeAuthority + "/..", invalid);
	   testUrl(schemeAuthority + "/../", invalid);
	   testUrl(schemeAuthority + "/test1/", valid);
	   testUrl(schemeAuthority + "/#", invalid);
	   testUrl(schemeAuthority + "", valid);
	   testUrl(schemeAuthority + "/test1/file", valid);
	   testUrl(schemeAuthority + "/t123/file", valid);
	   testUrl(schemeAuthority + "/$23/file", valid);
	   testUrl(schemeAuthority + "/../file", invalid);
	   testUrl(schemeAuthority + "/..//file", invalid);
	   testUrl(schemeAuthority + "/test1//file", valid);
	   testUrl(schemeAuthority + "/#/file", invalid);
	   testUrl("file:///test1.jpg", valid);
   }
   public void testYourFifthPartition(){
	   //Query
	   //QUERY_REGEX = "^(\\S*)$"
	   //                1
	   //	Matching from the beginning of the string, in order:
	   //	$1 Zero or more of any char that is not whitespace
	   //Per URL Regex on line 110 of UrlValidator.java, query must be preceded by "?"
	   System.out.println("\n****** Query ******");
	   testUrl(validUrlNoQuery += "?action=view", valid);
	   testUrl(validUrlNoQuery += "?action=edit&mode=up", valid);
	   testUrl(schemeAuthority += "?action=view", valid);
	   testUrl(schemeAuthority += "?action=edit&mode=up", valid);
   }
   
   public void testYourSixthPartition() {
	   //Fragment
	   //Per URL Regex on line 110 of UrlValidator.java, fragment must be preceded by "#"
	   //Can include any character that is not a line break
	   System.out.println("\n****** Fragment ******");
	   testUrl(validUrlNoQuery += "#Fragment", valid);
	   testUrl(validUrlNoQuery += "#", valid);
	   testUrl(validUrlNoQuery += "##", valid);
	   testUrl(validUrlNoQuery += "# ", valid);
	   testUrl(validUrlNoQuery += "#\\n", invalid);
   }
   
   public void testIsValid()
   {   
	   
	   //create arrays for the parts 
	   String[] schemeGood = {"http://", "h3tp://", ""};
	   String[] authGood = {"go.com", "0.0.0.0", "255.255.255.255", "www.google.com", "www.go.com"};
	   String[] portGood = {":80", ":65535", ":0", "", ":8080"};
	   String[] pathGood = {"", "/images", "/files/images"};
	   String[] optGood = {"", "/images", "/files/images"};
	   String[] queryGood = {"", "/?action=view", "/?action=view&start=5s"};
	   
	   //initialize variables
	   int possSize = 2025; //the # of times to cycle through the parts
	   //this value is the product of the number of possibilities given above
	   int resultSize = 0; 
	   String testString;
	   UrlValidator validCheck = new UrlValidator();
	   boolean result;
	   
	   //create an array to store the bad URLs from the tests
	   String[] badResults = new String[possSize];

	   
	   for(int a = 0; a < 3; a++)
	   {
		   for (int b = 0; b < 5; b++)
		   {
			   for (int c = 0; c < 5; c++)
			   {
				   for (int d = 0; d < 3; d++)
				   {
					   for (int e = 0; e < 3; e++)
					   {
						   for (int f = 0; f < 3; f++)
						   {
							   testString = schemeGood[a] + authGood[b] + portGood[c] + pathGood[d] + optGood[e] + queryGood[f];
							   result = validCheck.isValid(testString);
							   
							   //if we get back a false result, store the string
							   if (!result)
							   {
								   badResults[resultSize] = testString;
								   resultSize++;
							   }
						   }
					   }
				   }
			   }
		   }
		   
	   }
	   
	   //print out the results
	   if (resultSize == 0)
	   {
		   System.out.println("\nNo bugs found on this run through.\n");
	   }
	   else
	   {
		   System.out.println("\nErroneously Invalid URLs: \n");
		   for (int count = 0; count < resultSize; count++)
		   {
			   System.out.println(badResults[count] + "\n");			   
		   }
	   }

   }
 
		   
   String schemeBad[] = {"3ht://", "http:/", "http:", "http/", "://"};
}