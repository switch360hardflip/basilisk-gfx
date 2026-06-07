<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="@*|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|node()"/>
		</xsl:copy>
	</xsl:template>

	<xsl:template match="vectorTemplate">
		<function name="bs_v{.}Add">
			<return>void</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>a</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>b</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_add(a->a, b->a, out->a);</body>
		</function>
		<function name="bs_v{.}Sub">
			<return>void</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>a</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>b</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_sub(a->a, b->a, out->a);</body>
		</function>
		<function name="bs_v{.}Mul">
			<return>void</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>a</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>b</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_mul(a->a, b->a, out->a);</body>
		</function>
		<function name="bs_v{.}Div">
			<return>void</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>a</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>b</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_div(a->a, b->a, out->a);</body>
		</function>
		<function name="bs_v{.}MulV1">
			<return>void</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>v</name></param>
			<param><type>float</type><name>s</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_scale(v->a, s, out->a);</body>
		</function>
		<function name="bs_v{.}DivV1">
			<return>void</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>v</name></param>
			<param><type>float</type><name>s</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_scale(v->a, 1.0 / s, out->a);</body>
		</function>
		<function name="bs_v{.}Dot">
			<return>float</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>a</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>b</name></param>
			<body>glm_vec<xsl:value-of select="."/>_dot(a->a, b->a, out->a);</body>
		</function>
		<function name="bs_v{.}Normalize">
			<return>float</return>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>v</name></param>
			<body>glm_vec<xsl:value-of select="."/>_normalize(v->a);</body>
		</function>
		<function name="bs_v{.}NormalizeTo">
			<return>float</return>
			<param><type>const bs_vec<xsl:value-of select="."/>*</type><name>v</name></param>
			<param><type>bs_vec<xsl:value-of select="."/>*</type><name>out</name></param>
			<body>glm_vec<xsl:value-of select="."/>_normalize_to(v->a, out->a);</body>
		</function>
	</xsl:template>

</xsl:stylesheet>