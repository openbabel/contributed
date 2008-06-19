<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes"/> 

  <xsl:template match="/">
    <html>
      <body>
        <font size="12">OpenBabel Unit Test Results</font>
        <center>
        <table border="1" >
	  <tr bgcolor="#AAAAAA">
	    <td>Name Unit Test</td>
	    <td>Status</td>
	    <td># passed</td>
	    <td># failed</td>
	    <td># expected to fail</td>
	  </tr>
          <xsl:apply-templates/>
        </table>
	</center>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="TestSuite/TestCase">
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
