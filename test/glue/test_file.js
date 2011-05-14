suite.testFile = {};

suite.testFile.testExists = function() {
    assert.ok(File.exists('/tmp/test-exists'));
    assert.ok(!File.exists('/tmp/test-not-exists'));
};

suite.testFile.testIsDirectory = function() {
    assert.ok(!File.isDirectory('/tmp/test-file'));
    assert.ok(File.isDirectory('/tmp/test-dir'));
}

suite.testFile.testIsFile = function() {
    assert.ok(File.isFile('/tmp/test-file'));
    assert.ok(!File.isFile('/tmp/test-dir'));
}

suite.testFile.testRead = function() {
    assert.equal(File.read("/tmp/test-file"), "foobar\n");
    assert.equal(File.read("/tmp/test-notexists"), null);
}