// Copyright (c) 2009 - Decho Corp.

#include "mordor/common/uri.h"
#include "mordor/test/test.h"

static void serializeAndParse(const char *uri, const char *expected = NULL)
{
    if (!expected) expected = uri;
    TEST_ASSERT_EQUAL(URI(uri).toString(), expected);
}

TEST_WITH_SUITE(URI, serializationAndParsing)
{
    TEST_ASSERT_EQUAL(URI::Path("/a/b/c/./../../g").toString(),
        "/a/b/c/./../../g");
    serializeAndParse("example://a/b/c/%7Bfoo%7D");
    serializeAndParse("eXAMPLE://a/./b/../b/%63/%7bfoo%7d", "eXAMPLE://a/./b/../b/c/%7Bfoo%7D");
    serializeAndParse("http://a/b/c/d;p?q");
    serializeAndParse("g:h");
    serializeAndParse("g");
    serializeAndParse("http://a/b/c/g");
    serializeAndParse("./g");
    serializeAndParse("g/");
    serializeAndParse("http://a/b/c/g/");
    serializeAndParse("/g");
    serializeAndParse("http://a/g");
    serializeAndParse("//g");
    serializeAndParse("http://g");
    serializeAndParse("?y");
    serializeAndParse("http://a/b/c/d;p?y");
    serializeAndParse("g?y");
    serializeAndParse("http://a/b/c/g?y");
    serializeAndParse("#s");
    serializeAndParse("http://a/b/c/d;p?q#s");
    serializeAndParse("g#s");
    serializeAndParse("http://a/b/c/g#s");
    serializeAndParse("g?y#s");
    serializeAndParse("http://a/b/c/g?y#s");
    serializeAndParse(";x");
    serializeAndParse("http://a/b/c/;x");
    serializeAndParse("g;x");
    serializeAndParse("http://a/b/c/g;x");
    serializeAndParse("g;x?y#s");
    serializeAndParse("http://a/b/c/g;x?y#s");
    serializeAndParse("");
    serializeAndParse("http://a/b/c/d;p?q");
    serializeAndParse(".");
    serializeAndParse("http://a/b/c/");
    serializeAndParse("./");
    serializeAndParse("..");
    serializeAndParse("http://a/b/");
    serializeAndParse("../");
    serializeAndParse("../g");
    serializeAndParse("http://a/b/g");
    serializeAndParse("../..");
    serializeAndParse("http://a/");
    serializeAndParse("../../");
    serializeAndParse("../../g");
    serializeAndParse("http://a/g");
    serializeAndParse("../../../g");
    serializeAndParse("../../../../g");
    serializeAndParse("/./g");
    serializeAndParse("/../g");
    serializeAndParse("g.");
    serializeAndParse("http://a/b/c/g.");
    serializeAndParse(".g");
    serializeAndParse("http://a/b/c/.g");
    serializeAndParse("g..");
    serializeAndParse("http://a/b/c/g..");
    serializeAndParse("..g");
    serializeAndParse("http://a/b/c/..g");
    serializeAndParse("./../g");
    serializeAndParse("./g/.");
    serializeAndParse("http://a/b/c/g/");
    serializeAndParse("g/./h");
    serializeAndParse("http://a/b/c/g/h");
    serializeAndParse("g/../h");
    serializeAndParse("http://a/b/c/h");
    serializeAndParse("g;x=1/./y");
    serializeAndParse("http://a/b/c/g;x=1/y");
    serializeAndParse("g;x=1/../y");
    serializeAndParse("http://a/b/c/y");
    serializeAndParse("g?y/./x");
    serializeAndParse("http://a/b/c/g?y/./x");
    serializeAndParse("g?y/../x");
    serializeAndParse("http://a/b/c/g?y/../x");
    serializeAndParse("g#s/./x");
    serializeAndParse("http://a/b/c/g#s/./x");
    serializeAndParse("g#s/../x");
    serializeAndParse("http://a/b/c/g#s/../x");
    serializeAndParse("http:g");
}

TEST_WITH_SUITE(URI, pathNormalization)
{
    URI::Path p("/a/b/c/./../../g");
    p.removeDotComponents();
    TEST_ASSERT_EQUAL(p, URI::Path("/a/g"));
}

TEST_WITH_SUITE(URI, normalization)
{
    URI lhs("example://a/b/c/%7Bfoo%7D");
    URI rhs("eXAMPLE://a/./b/../b/%63/%7bfoo%7d");

    lhs.normalize();
    rhs.normalize();

    TEST_ASSERT(lhs.isDefined());
    TEST_ASSERT(rhs.isDefined());
    TEST_ASSERT(lhs.schemeDefined());
    TEST_ASSERT(rhs.schemeDefined());
    TEST_ASSERT_EQUAL(lhs.scheme(), rhs.scheme());
    TEST_ASSERT(!lhs.authority.portDefined());
    TEST_ASSERT(!rhs.authority.portDefined());
    TEST_ASSERT(lhs.authority.hostDefined());
    TEST_ASSERT(rhs.authority.hostDefined());
    TEST_ASSERT_EQUAL(lhs.authority.host(), rhs.authority.host());
    TEST_ASSERT(!lhs.authority.userinfoDefined());
    TEST_ASSERT(!rhs.authority.userinfoDefined());
    TEST_ASSERT_EQUAL(lhs.authority, rhs.authority);
    TEST_ASSERT_EQUAL(lhs.path.type, rhs.path.type);
    TEST_ASSERT_EQUAL(lhs.path.segments, rhs.path.segments);
    TEST_ASSERT_EQUAL(lhs.path, rhs.path);
    TEST_ASSERT(!lhs.queryDefined());
    TEST_ASSERT(!rhs.queryDefined());
    TEST_ASSERT(!lhs.fragmentDefined());
    TEST_ASSERT(!rhs.fragmentDefined());
    TEST_ASSERT_EQUAL(lhs, rhs);
}

TEST_WITH_SUITE(URI, transform)
{
    URI base("http://a/b/c/d;p?q");
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g:h")), URI("g:h"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g")), URI("http://a/b/c/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("./g")), URI("http://a/b/c/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g/")), URI("http://a/b/c/g/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("/g")), URI("http://a/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("//g")), URI("http://g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("?y")), URI("http://a/b/c/d;p?y"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g?y")), URI("http://a/b/c/g?y"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("#s")), URI("http://a/b/c/d;p?q#s"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g#s")), URI("http://a/b/c/g#s"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g?y#s")), URI("http://a/b/c/g?y#s"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI(";x")), URI("http://a/b/c/;x"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g;x")), URI("http://a/b/c/g;x"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g;x?y#s")), URI("http://a/b/c/g;x?y#s"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("")), URI("http://a/b/c/d;p?q"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI(".")), URI("http://a/b/c/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("./")), URI("http://a/b/c/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("..")), URI("http://a/b/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../")), URI("http://a/b/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../g")), URI("http://a/b/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../..")), URI("http://a/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../../")), URI("http://a/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../../g")), URI("http://a/g"));
    
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../../../g")), URI("http://a/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("../../../../g")), URI("http://a/g"));
    
    TEST_ASSERT_EQUAL(URI::transform(base, URI("/./g")), URI("http://a/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("/../g")), URI("http://a/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g.")), URI("http://a/b/c/g."));
    TEST_ASSERT_EQUAL(URI::transform(base, URI(".g")), URI("http://a/b/c/.g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g..")), URI("http://a/b/c/g.."));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("..g")), URI("http://a/b/c/..g"));
    
    TEST_ASSERT_EQUAL(URI::transform(base, URI("./../g")), URI("http://a/b/g"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("./g/.")), URI("http://a/b/c/g/"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g/./h")), URI("http://a/b/c/g/h"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g/../h")), URI("http://a/b/c/h"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g;x=1/./y")), URI("http://a/b/c/g;x=1/y"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g;x=1/../y")), URI("http://a/b/c/y"));
    
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g?y/./x")), URI("http://a/b/c/g?y/./x"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g?y/../x")), URI("http://a/b/c/g?y/../x"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g#s/./x")), URI("http://a/b/c/g#s/./x"));
    TEST_ASSERT_EQUAL(URI::transform(base, URI("g#s/../x")), URI("http://a/b/c/g#s/../x"));
    
    TEST_ASSERT_EQUAL(URI::transform(base, URI("http:g")), URI("http:g"));
}
