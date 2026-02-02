# Contributing to Apache Axis2/C

We welcome contributions to Apache Axis2/C! This guide explains how to get involved.

## Ways to Contribute

- **Bug Reports** - Report issues you encounter
- **Bug Fixes** - Submit patches for known issues
- **Documentation** - Improve guides and API docs
- **Features** - Implement new functionality
- **Code Review** - Review pull requests from others

## Getting Started

### 1. Fork the Repository

```bash
# Fork on GitHub, then clone your fork
git clone https://github.com/YOUR_USERNAME/axis-axis2-c-core.git
cd axis-axis2-c-core
git remote add upstream https://github.com/apache/axis-axis2-c-core.git
```

### 2. Create a Branch

```bash
git checkout -b feature/my-feature
# or
git checkout -b fix/issue-123
```

### 3. Make Changes

- Follow the [Coding Conventions](../guide/coding-conventions.md)
- Write tests for new functionality
- Update documentation as needed

### 4. Test Your Changes

Build and test following the instructions in the `INSTALL` file:

```bash
./configure [options]
make
make check
```

### 5. Commit and Push

```bash
git add .
git commit -m "Brief description of changes"
git push origin feature/my-feature
```

### 6. Submit a Pull Request

1. Go to your fork on GitHub
2. Click "New Pull Request"
3. Select your branch
4. Fill out the PR template
5. Submit for review

## Pull Request Guidelines

### Checklist

- [ ] Code follows [coding conventions](../guide/coding-conventions.md)
- [ ] Tests added/updated for new functionality
- [ ] Documentation updated if needed
- [ ] Commit messages are clear and descriptive
- [ ] No unrelated changes included

### PR Title Format

```
[AXIS2C-XXX] Brief description
```

Or for non-JIRA issues:

```
fix: Description of bug fix
feat: Description of new feature
docs: Documentation update
```

### Review Process

1. A maintainer will review your PR
2. Address any requested changes
3. Once approved, a maintainer will merge

## Code Style

See [Coding Conventions](../guide/coding-conventions.md) for:

- Naming conventions
- Code formatting
- Memory management patterns
- Error handling

## Testing

### Running Tests

```bash
make check
```

### Writing Tests

- Place tests in the `test/` directory
- Use the existing test framework
- Cover both success and failure cases

## Building the Documentation Site

The documentation site uses [MkDocs](https://www.mkdocs.org/) with the Material theme.

### Setup

```bash
# Create a Python virtual environment and install dependencies
python3 -m venv mkdocs-venv
./mkdocs-venv/bin/pip install mkdocs mkdocs-material
```

### Preview Locally

```bash
# Start the development server
./mkdocs-venv/bin/mkdocs serve

# Or specify a different port
./mkdocs-venv/bin/mkdocs serve -a localhost:8080
```

Then open [http://localhost:8000](http://localhost:8000) in your browser. The site auto-reloads when you edit Markdown files.

### Build Static Site

```bash
# Generate static HTML in site/ directory
./mkdocs-venv/bin/mkdocs build
```

### Documentation Structure

```
docs/
├── index.md                 # Home page
├── getting-started/         # Installation and quick start
├── guide/                   # User and developer guides
├── api/                     # API reference
├── community/               # Contributing, mailing lists
└── *.md                     # HTTP/2, security docs
```

## Communication

- **Developer List**: [c-dev@axis.apache.org](mailto:c-dev@axis.apache.org)
- **User List**: [c-user@axis.apache.org](mailto:c-user@axis.apache.org)
- **Jira**: [issues.apache.org/jira/projects/AXIS2C](https://issues.apache.org/jira/projects/AXIS2C) for bug reports and feature requests

## License

By contributing, you agree that your contributions will be licensed under the [Apache License 2.0](license.md).

## Code of Conduct

All contributors must follow the [Apache Code of Conduct](https://www.apache.org/foundation/policies/conduct.html).

Thank you for contributing to Apache Axis2/C!
