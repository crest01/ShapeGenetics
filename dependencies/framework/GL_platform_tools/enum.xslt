<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template name="enum-write-declaration">
		<xsl:choose>
			<xsl:when test="@type='ull'">
				<xsl:text>const unsigned long long </xsl:text>
			</xsl:when>
			<xsl:when test="../@type='bitmask'">
				<xsl:text>const GLbitfield </xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>const GLenum </xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:value-of select="@name" />
		<xsl:text> = </xsl:text>
		<xsl:value-of select="@value" />
		<xsl:value-of select="translate(@type, 'ul', 'UL')" />
		<xsl:text>;
</xsl:text>
	</xsl:template>

</xsl:stylesheet>
