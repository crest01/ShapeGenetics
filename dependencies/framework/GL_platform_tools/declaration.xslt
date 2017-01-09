<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template name="declaration-write-remaining-tokens">
		<xsl:param name="tokens" />
		<xsl:variable name="stream" select="normalize-space($tokens)" />
		<xsl:if test="$stream">
			<xsl:choose>
				<xsl:when test="starts-with($stream, '*')">
					<xsl:text>*</xsl:text>
					<xsl:call-template name="declaration-write-remaining-tokens">
						<xsl:with-param name="tokens" select="substring($stream, 2)" />
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text> </xsl:text>
					<xsl:value-of select="substring-before(concat($stream, ' '), ' ')" />
					<xsl:call-template name="declaration-write-remaining-tokens">
						<xsl:with-param name="tokens" select="substring-after($stream, ' ')" />
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:if>
	</xsl:template>

	<xsl:template name="declaration-write-tokens">
		<xsl:param name="tokens" />
		<xsl:variable name="stream" select="normalize-space($tokens)" />
		<xsl:value-of select="substring-before(concat($stream, ' '), ' ')" />
		<xsl:call-template name="declaration-write-remaining-tokens">
			<xsl:with-param name="tokens" select="substring-after($stream, ' ')" />
		</xsl:call-template>
	</xsl:template>

	<xsl:template name="declaration-write-type">
		<xsl:param name="decl" />
		<xsl:choose>
			<xsl:when test="self::name">
				<xsl:call-template name="declaration-write-tokens">
					<xsl:with-param name="tokens" select="$decl" />
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="acc" select="concat($decl, .)" />
				<xsl:for-each select="(following-sibling::*|following-sibling::node())[1]">
					<xsl:call-template name="declaration-write-type">
						<xsl:with-param name="decl" select="$acc" />
					</xsl:call-template>
				</xsl:for-each>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="declaration-write">
		<xsl:for-each select="(*|node())[1]">
			<xsl:call-template name="declaration-write-type" />
		</xsl:for-each>
		<xsl:text> </xsl:text>
		<xsl:value-of select="name" />
	</xsl:template>

</xsl:stylesheet>
