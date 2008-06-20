<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes"/> 

  <xsl:template match="TestSuite">
    <!-- <xsl:if test="@name = 'OpenBabelTestSuite'"> -->
    <style type="text/css">
      table.test_suite {
        border-width: 0px 0px 0px 0px;
	border-spacing: 1px;
      }
      table.test_suite th {
        border-width: 1px 1px 1px 1px;
	padding: 5px 5px 5px 5px;
	border-style: solid solid solid solid;
	border-color: black black black black;
      }
      table.test_suite td {
	border-width: 1px 1px 1px 1px;
	padding: 5px 5px 5px 5px;
 	border-style: solid solid solid solid;
	border-color: black black black black;
      }
      table.test_case {
        border-width: 0px 0px 0px 0px;
	border-spacing: 1px;
      }
      table.test_case th {
        border-width: 1px 1px 1px 1px;
	padding: 5px 5px 5px 5px;
	border-style: solid solid solid solid;
	border-color: black black black black;
	background-color: rgb(0, 100, 200);
      }
      table.test_case td {
	border-width: 1px 1px 1px 1px;
	padding: 5px 5px 5px 5px;
 	border-style: solid solid solid solid;
	border-color: black black black black;
      }
 
    </style>
    <html>
      <body>
        <center>
        <font size="12">OpenBabel Unit Test Results</font>
        <table border="1" class="test_suite">
	  <tr><th>result</th><td><xsl:value-of select="@result" /></td></tr>
	  <tr><th># assertions passed</th><td><xsl:value-of select="@assertions_passed" /></td></tr>
	  <tr><th># assertions failed</th><td><xsl:value-of select="@assertions_failed" /></td></tr>
	  <tr><th># expected failures</th><td><xsl:value-of select="@expected_failures" /></td></tr>
	  <tr><th># test cases passed</th><td><xsl:value-of select="@test_cases_passed" /></td></tr>
	  <tr><th># test cases failed</th><td><xsl:value-of select="@test_cases_failed" /></td></tr>
	  <tr><th># test cases skipped</th><td><xsl:value-of select="@test_cases_skipped" /></td></tr>
	  <tr><th># test cases aborted</th><td><xsl:value-of select="@test_cases_aborted" /></td></tr>
        </table>
        <table border="1" class="test_case">
	  <tr bgcolor="#AAAAAA">
	    <th>Name Unit Test</th>
	    <th>Status</th>
	    <th># passed</th>
	    <th># failed</th>
	    <th># expected to fail</th>
	  </tr>
          <xsl:apply-templates select="TestSuite/TestCase"/>
        </table>
 	</center>
      </body>
    </html>
    <!-- </xsl:if> -->
  </xsl:template>
  
  <xsl:template  match="TestSuite/TestCase">
      <tr>
        <td>
          <xsl:value-of select="@name" />
	</td>
	<td>
          <xsl:if test="@result = 'passed'"><font color="green"><b>success</b></font></xsl:if>
          <xsl:if test="@result != 'passed'"><font color="red"><b>failure</b></font></xsl:if>
	</td>
	<td>
          <xsl:value-of select="@assertions_passed" />
	</td>
	<td>
          <xsl:value-of select="@assertions_failed" />
	</td>
	<td>
          <xsl:value-of select="@expected_failures" />
	</td>
      </tr>
  </xsl:template>
</xsl:stylesheet>
