<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:import href="declaration.xslt" />

	<xsl:template name="command-write-following-function-param-list">
		<xsl:for-each select="following-sibling::param">
			<xsl:text>, </xsl:text>
			<xsl:call-template name="declaration-write" />
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="command-write-function-param-list">
		<xsl:for-each select="param[1]">
			<xsl:call-template name="declaration-write" />
			<xsl:call-template name="command-write-following-function-param-list" />
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="command-write-remaining-function-call-arg-list">
		<xsl:for-each select="following-sibling::param">
			<xsl:text>, </xsl:text>
			<xsl:value-of select="name" />
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="command-write-function-call-arg-list">
		<xsl:for-each select="param[1]">
			<xsl:value-of select="name" />
			<xsl:call-template name="command-write-remaining-function-call-arg-list" />
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="command-write-function-signature">
		<xsl:param name="calling-convention" select="''" />
		<xsl:param name="identifier" select="proto/name" />
		<xsl:if test="$calling-convention != ''">
			<xsl:value-of select="$calling-convention" />
			<xsl:text> </xsl:text>
		</xsl:if>
		<xsl:value-of select="$identifier" />
		<xsl:text>(</xsl:text>
		<xsl:call-template name="command-write-function-param-list" />
		<xsl:text>)</xsl:text>
	</xsl:template>

	<xsl:template name="command-write-function-pointer">
		<xsl:param name="calling-convention" select="''" />
		<xsl:text>(</xsl:text>
		<xsl:choose>
			<xsl:when test="$calling-convention != ''">
				<xsl:value-of select="$calling-convention" />
				<xsl:text>* </xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>*</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:value-of select="proto/name" />
		<xsl:text>_t)</xsl:text>
		<xsl:text>(</xsl:text>
		<xsl:call-template name="command-write-function-param-list" />
		<xsl:text>)</xsl:text>
	</xsl:template>

	<xsl:template name="command-write-function-return-type">
		<xsl:for-each select="(proto/*|proto/node())[1]">
			<xsl:call-template name="declaration-write-type" />
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="command-write-function-pointer-declaration">
		<xsl:param name="calling-convention" select="''" />
		<xsl:param name="identifier" select="proto/name" />
		<xsl:call-template name="command-write-function-return-type" />
		<xsl:text> </xsl:text>
		<xsl:call-template name="command-write-function-pointer">
			<xsl:with-param name="calling-convention" select="$calling-convention" />
			<xsl:with-param name="identifier" select="$identifier" />
		</xsl:call-template>
		<xsl:text>;
</xsl:text>
	</xsl:template>

	<xsl:template name="command-write-function-declaration">
		<xsl:param name="calling-convention" select="''" />
		<xsl:param name="identifier" select="proto/name" />
		<xsl:call-template name="command-write-function-return-type" />
		<xsl:text> </xsl:text>
		<xsl:call-template name="command-write-function-signature">
			<xsl:with-param name="calling-convention" select="$calling-convention" />
			<xsl:with-param name="identifier" select="$identifier" />
		</xsl:call-template>
		<xsl:text>;
</xsl:text>
	</xsl:template>

	<xsl:template name="command-write-function-call">
		<xsl:param name="identifier" select="proto/name" />
		<xsl:value-of select="$identifier" />
		<xsl:text>(</xsl:text>
		<xsl:call-template name="command-write-function-call-arg-list" />
		<xsl:text>)</xsl:text>
	</xsl:template>
	
</xsl:stylesheet>
