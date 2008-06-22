<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes"/> 

  <xsl:template match="TestSuite">
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
        <font size="12">OpenBabel Unit Test Log</font>
          <xsl:apply-templates select="TestLog"/>
 	</center>
      </body>
    </html>
  </xsl:template>
  
  <xsl:template  match="TestLog">
    <a name="{@file}"/>
    <table border="1" width="100%" class="test_suite">
      <xsl:for-each select="Error">
        <tr>
          <th width="10%">
            <xsl:value-of select="@file" />:<xsl:value-of select="@line" />
          </th>
          <td>
            <xsl:value-of select="." />
          </td>
        </tr>
      </xsl:for-each>
      <xsl:for-each select="FatalError">
        <tr bgcolor="#CC0000">
          <th width="10%">
            <xsl:value-of select="@file" />:<xsl:value-of select="@line" />
          </th>
          <td>
            <xsl:value-of select="." />
          </td>
        </tr>
      </xsl:for-each>
      <!--
      <a name="{@name}_passed"/>
      <xsl:for-each select="Info">
        <tr bgcolor="#00CC00">
          <td width="10%">
            line <xsl:value-of select="@line" />
          </td>
          <td>
            <xsl:value-of select="." />
          </td>
        </tr>
      </xsl:for-each>
      <a name="{@name}_cout"/>
      <tr>
        <td valign="top" width="10%">
          std::cout
        </td>
        <td>
          <xsl:value-of select="." />
        </td>
      </tr>
      -->
    </table>
  </xsl:template>
  
</xsl:stylesheet>
