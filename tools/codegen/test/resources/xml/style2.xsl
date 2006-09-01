<?xml version="1.0"?>
<xsl:stylesheet version="1.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="/">
      <html>
        <head>
          <title>Atomic Number vs. Atomic Weight</title>
        </head>
        <body>
          <xsl:apply-templates select="PERIODIC_TABLE"/>
        </body>
      </html>
    </xsl:template>
    <xsl:template match="PERIODIC_TABLE">
      <h1>Atomic Number vs. Atomic Weight</h1>
      <table>
        <th>Element</th>
        <th>Atomic Number</th>
        <th>Atomic Weight</th>
          <xsl:apply-templates select="ATOM"/>
      </table>
    </xsl:template>
    <xsl:template match="ATOM">
      <tr>
        <td><xsl:value-of select="NAME"/></td>
        <td><xsl:value-of select="ATOMIC_NUMBER"/></td>
        <td><xsl:value-of select="ATOMIC_WEIGHT"/></td>
      </tr>
    </xsl:template>
</xsl:stylesheet>
