# Axis2/C Release Process

This document describes the Apache Software Foundation (ASF) release process for Apache Axis2/C. All releases must comply with [ASF Release Policy](https://www.apache.org/legal/release-policy.html).

## Prerequisites

### Release Manager Requirements

The Release Manager (RM) must:

1. **Be an Apache committer** with write access to the repository
2. **Have a PGP key** that meets [ASF requirements](https://www.apache.org/dev/release-signing.html):
   - RSA key with at least 4096 bits (recommended)
   - Key must be signed by other Apache committers (web of trust)
   - Public key must be uploaded to public key servers

3. **Export and register your PGP key**:
   ```bash
   # Export your public key
   gpg --armor --export your-key-id >> KEYS

   # Verify key is available
   gpg --list-secret-keys
   ```

4. **Append your key to the KEYS file** at:
   - `https://dist.apache.org/repos/dist/release/axis/axis2/c/core/KEYS`
   - `https://dist.apache.org/repos/dist/dev/axis/axis2/c/core/KEYS`

### Build Environment

Ensure you have:

- GCC or Clang compiler
- Autotools (autoconf, automake, libtool)
- OpenSSL development headers
- Apache httpd development headers (for mod_axis2)
- Optional: libxml2, json-c, nghttp2

## Release Process Overview

1. Prepare the release branch
2. Update version numbers and documentation
3. Build and test source distribution
4. Sign artifacts and generate checksums
5. Upload to ASF staging area
6. Call for vote
7. After vote passes: promote release
8. Deploy updated website
9. Announce release

### Source-Only Releases

Axis2/C follows the same release model as [Apache httpd](https://httpd.apache.org/dev/release.html): **source-only releases**.

Per the [ASF Release Policy](https://www.apache.org/legal/release-policy.html):

> "Every ASF release **MUST** contain one or more source packages... binary/bytecode packages **MAY** be distributed alongside official Apache releases."

The Apache HTTP Server Project explicitly states:

> "The Apache HTTP Server Project itself does not provide binary releases of software, only source code."

**Why source-only for Axis2/C:**

- **ASF Policy Compliance**: Source releases are required; binaries are optional
- **Apache httpd Precedent**: The flagship ASF C project releases source-only
- **ABI Complexity**: C binary compatibility varies by compiler, flags, and linked libraries
- **Platform Diversity**: Maintaining binaries for Linux, macOS, FreeBSD, Android is impractical
- **Modern Practice**: Users install via system package managers (apt, yum, brew) or build from source
- **Enterprise Expectations**: Production deployments typically build from source with custom configurations

Users who need pre-built binaries should use their distribution's package manager or build from source.

### Note on Nexus Staging Repository

Unlike Java projects (such as Axis2/Java) which use the [Nexus staging repository](https://repository.apache.org) to publish Maven artifacts to Maven Central, Axis2/C is a native C library that does not produce Maven artifacts.

For C projects, the release distribution mechanism is:

- **Source tarballs** are uploaded directly to `dist.apache.org` SVN repositories
- There is no Nexus step required for Axis2/C releases

This is consistent with Apache httpd and other Apache C projects.

## Step 1: Prepare the Release

### 1.1 Create a Release Branch (Optional)

For major releases, consider creating a release branch:

```bash
git checkout master
git pull origin master
git checkout -b release/2.0.0
```

For minor/patch releases, you may release directly from master.

### 1.2 Update Version Numbers

Update version in these files:

| File | Description |
|------|-------------|
| `configure.ac` | `AC_INIT(axis2c-src, X.Y.Z)` and `VERSION_NO` |
| `util/configure.ac` | `VERSION_NO` |
| `axiom/configure.ac` | `VERSION_NO` |
| `guththila/configure.ac` | `VERSION_NO` |
| `neethi/configure.ac` | `VERSION_NO` |
| `samples/configure.ac` | Version references |
| `NOTICE` | Copyright year range |
| `NEWS` | Release notes summary |
| `doxygenconf` | `PROJECT_NUMBER` |

### 1.3 Update Documentation

- Update `docs/release-notes-X.Y.Z.md` with resolved issues
- Review and update `INSTALL` file if needed
- Verify all documentation links work

### 1.4 Regenerate Build System

```bash
./autogen.sh
```

## Step 2: Build and Test

### 2.1 Build from Source

```bash
./configure --prefix=/tmp/axis2c-test \
    --enable-tests=yes \
    --with-apache2=/usr/include/apache2
make clean
make
make check
make install
```

### 2.2 Run Test Suite

```bash
make check
```

Verify all tests pass. Fix any failures before proceeding.

### 2.3 Create Source Distribution

```bash
make dist
# Creates: axis2c-src-X.Y.Z.tar.gz
```

### 2.4 Verify Source Distribution Builds

```bash
# Extract and build from source tarball
tar xzf axis2c-src-X.Y.Z.tar.gz
cd axis2c-src-X.Y.Z
./configure --prefix=/tmp/axis2c-verify
make
make check
```

## Step 3: Sign and Checksum

ASF releases require GPG signatures and SHA-512 checksums (MD5 and SHA-1 are no longer acceptable).

### 3.1 Sign Artifacts

```bash
#!/bin/bash
VERSION="2.0.0"

# Sign source distribution
gpg --armor --detach-sign axis2c-src-${VERSION}.tar.gz
```

### 3.2 Create SHA-512 Checksums

```bash
#!/bin/bash
VERSION="2.0.0"

sha512sum axis2c-src-${VERSION}.tar.gz > axis2c-src-${VERSION}.tar.gz.sha512
```

### 3.3 Verify Signatures

```bash
# Verify GPG signatures
gpg --verify axis2c-src-${VERSION}.tar.gz.asc axis2c-src-${VERSION}.tar.gz

# Verify checksums
sha512sum -c axis2c-src-${VERSION}.tar.gz.sha512
```

## Step 4: Upload to Staging

### 4.1 Upload to ASF Development Area

Use SVN to upload to the development distribution area. Note the SVN dev path
does NOT include `core/` — it is `dist/dev/axis/axis2/c/`:

```bash
# Checkout dev area (empty directory, creates it if needed)
svn checkout https://dist.apache.org/repos/dist/dev/axis/axis2/c/ ~/repos/axis2c-dev-dist
cd ~/repos/axis2c-dev-dist

# Create version directory and copy artifacts
mkdir ${VERSION}
cp ~/repos/axis-axis2-c-core/axis2c-src-${VERSION}.tar.gz     ${VERSION}/
cp ~/repos/axis-axis2-c-core/axis2c-src-${VERSION}.tar.gz.asc ${VERSION}/
cp ~/repos/axis-axis2-c-core/axis2c-src-${VERSION}.tar.gz.sha512 ${VERSION}/

# Commit
svn add ${VERSION}
svn commit -m "Stage Apache Axis2/C ${VERSION} RC1 for vote"
```

Do NOT copy KEYS into the version directory — the KEYS file lives at the
parent level (`dist/release/axis/axis2/c/KEYS`) and is already maintained.

### 4.2 Build and Commit Site to core-staging

The site is built with MkDocs and committed to `axis2/c/core-staging/` in the
axis-site repo. ASF GitPubSub auto-publishes this to
`https://axis.apache.org/axis2/c/core-staging/` within minutes of the push.

This is a **temporary staging directory** used only during the release vote —
identical to the Axis2/Java core-staging pattern. Do NOT update `core/` yet;
that happens post-vote (Step 7).

```bash
# Install MkDocs if not already available
pip install mkdocs mkdocs-material

# Build site from axis-axis2-c-core docs/
cd ~/repos/axis-axis2-c-core
mkdocs build
# Output is in site/

# Sync built site into core-staging (does not touch production core/)
rsync -av ~/repos/axis-axis2-c-core/site/ ~/repos/site/axis-site/axis2/c/core-staging/

# Commit to axis-site master branch
cd ~/repos/site/axis-site
git add axis2/c/core-staging/
git commit -m "axis2/c core-staging for ${VERSION} release vote"
git push
```

Before building, ensure `docs/downloads.md` has the correct download links
for the new version pointing to `downloads.apache.org/axis/axis2/c/${VERSION}/`.

## Step 5: Call for Vote

### 5.1 Send Vote Email

Send a vote email to `c-dev@axis.apache.org`. Keep it brief — see the
Axis2/Java vote format as a model:

```
Subject: [VOTE] Release Apache Axis2/C X.Y.Z

I'd like to call a vote on releasing Apache Axis2/C X.Y.Z.

This is the first release of Axis2/C since 1.6.0 in 2009. Version X.Y.Z
includes 17 years of bug fixes (115+ Jira issues resolved), HTTP/2 support
with pure JSON-RPC services, and removes obsolete transports (TCP, CGI,
libcurl, AMQP, IIS/ISAPI).

Git tag: https://github.com/apache/axis-axis2-c-core/releases/tag/vX.Y.Z

Distributions: https://dist.apache.org/repos/dist/dev/axis/axis2/c/X.Y.Z/

Site: https://axis.apache.org/axis2/c/core-staging/

+1 from me

Your Name
```

Notes:
- The site link uses `core-staging/` — the temporary preview URL built in Step 4.2.
  This matches the Axis2/Java release pattern.
- After the vote passes, core-staging is promoted to core/ (Step 7) and deleted.
- The distributions path does NOT include `/core/`

Voting guide:
- Download and verify signatures: gpg --verify *.asc
- Verify checksums: sha512sum -c *.sha512
- Build from source and run tests
- Review release notes and documentation
```

### 5.2 Vote Requirements

Per ASF policy:
- Voting period must be **at least 72 hours**
- Requires **at least 3 binding +1 votes** from PMC members
- **No -1 votes** from PMC members (or issues must be resolved)

## Step 6: After Vote Passes

### 6.1 Tag the Release

```bash
git tag -s vX.Y.Z -m "Apache Axis2/C X.Y.Z release"
git push origin vX.Y.Z
```

### 6.2 Promote Artifacts to Release Area

```bash
# Move from dev to release
svn mv https://dist.apache.org/repos/dist/dev/axis/axis2/c/core/${VERSION} \
       https://dist.apache.org/repos/dist/release/axis/axis2/c/core/ \
       -m "Promote Axis2/C ${VERSION} release"
```

### 6.3 Update KEYS File

If new committer keys were added:

```bash
svn checkout https://dist.apache.org/repos/dist/release/axis/axis2/c/core/ axis2c-release
cd axis2c-release
# Append new keys
cat new_KEYS >> KEYS
svn commit -m "Update KEYS for ${VERSION} release"
```

### 6.4 Archive Old Releases

Per ASF policy, only keep the latest release in the main distribution area:

```bash
# Move old versions to archive
svn mv https://dist.apache.org/repos/dist/release/axis/axis2/c/core/OLD_VERSION \
       https://archive.apache.org/dist/axis/axis2/c/core/ \
       -m "Archive Axis2/C OLD_VERSION"
```

## Step 7: Deploy Website

### 7.1 Promote core-staging to core

After the vote passes, replace the production site with core-staging and delete
the staging directory:

```bash
cd ~/repos/site/axis-site

# Replace production site with staged content
rsync -av --delete axis2/c/core-staging/ axis2/c/core/

# Remove the staging directory
git rm -r axis2/c/core-staging

# Stage and commit
git add -A axis2/c/core/
git commit -m "Apache Axis2/C ${VERSION} site"
git push
```

ASF GitPubSub automatically deploys the commit.

### 7.2 Verify Site Deployment

Verify:
- https://axis.apache.org/axis2/c/core/
- https://axis.apache.org/axis2/c/core/downloads/

Deployment is typically within minutes but may take up to an hour.

## Step 8: Announce Release

### 8.1 Wait for Mirror Propagation

Wait for:
- Distributions available on mirrors (check https://downloads.apache.org/axis/axis2/c/core/)
- Site updated and accessible

### 8.2 Send Announcement

**To c-user@axis.apache.org and c-dev@axis.apache.org:**

```
Subject: [ANNOUNCE] Apache Axis2/C X.Y.Z Released

The Apache Axis2/C team is pleased to announce the release of
Apache Axis2/C X.Y.Z.

Apache Axis2/C is a high-performance web services framework
written in C, supporting HTTP/2, JSON-RPC, and legacy SOAP.

Download:
https://axis.apache.org/axis2/c/core/download.html

Release Notes:
https://axis.apache.org/axis2/c/core/release-notes-X.Y.Z.html

Key changes in this release:
- [List major changes]

Regards,
The Apache Axis2/C Team
```

**To announce@apache.org** (separate email, from @apache.org address):

```
Subject: [ANNOUNCE] Apache Axis2/C X.Y.Z Released

The Apache Axis2/C team is pleased to announce the release of
Apache Axis2/C version X.Y.Z.

Apache Axis2/C is a high-performance SOAP/HTTP web services
framework written in C. It provides support for WS-* specifications
through Rampart/C and modern HTTP/2 + JSON-RPC protocols.

Download: https://axis.apache.org/axis2/c/core/
Documentation: https://axis.apache.org/axis2/c/core/

Regards,
The Apache Axis2/C Team
```

## Step 9: Post-Release Tasks

### 9.1 Update JIRA

- Mark the version as released in JIRA
- Create the next version if not exists
- Close resolved issues for this release

### 9.2 Update DOAP File

If one exists, update `doap.rdf` with the new release information.

### 9.3 Merge Release Branch

If a release branch was used:

```bash
git checkout master
git merge release/X.Y.Z
git push origin master
```

### 9.4 Prepare for Next Release

- Update version in configure.ac to next SNAPSHOT/development version
- Create empty release notes file for next version

## Troubleshooting

### Vote Fails

If the vote fails:
1. Address all concerns raised during voting
2. Increment the release candidate number (rc2, rc3, etc.)
3. Upload new artifacts to dev area
4. Call for a new vote

### Build Issues

If builds fail on different platforms:
1. Set up CI testing before release
2. Consider using Docker for reproducible builds
3. Test on multiple platforms (Linux, macOS, FreeBSD)

### Signature Verification Fails

1. Ensure your key is uploaded to key servers
2. Verify KEYS file contains your public key
3. Re-sign artifacts if needed

## References

- [ASF Release Policy](https://www.apache.org/legal/release-policy.html)
- [ASF Release Publishing](https://infra.apache.org/release-publishing.html)
- [Release Signing](https://www.apache.org/dev/release-signing.html)
- [Apache Voting Process](https://www.apache.org/foundation/voting.html)
- [Apache httpd Release Guidelines](https://httpd.apache.org/dev/release.html) - Reference for C project source-only releases
- [Axis2/C Issue Tracker](https://issues.apache.org/jira/projects/AXIS2C)
