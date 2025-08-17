<a id="readme-top"></a>

<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/cubevlmu/HTMLParser">
    <img src="docs/logo.png" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">HTMLParser</h3>

  <p align="center">
    A fast and efficient HTML parsing library for C++.
    <br />
    <a href="https://github.com/cubevlmu/HTMLParser"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/cubevlmu/HTMLParser">View Demo</a>
    &middot;
    <a href="https://github.com/cubevlmu/HTMLParser/issues/new?labels=bug&template=bug-report---.md">Report Bug</a>
    &middot;
    <a href="https://github.com/cubevlmu/HTMLParser/issues/new?labels=enhancement&template=feature-request---.md">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

## About The Project

HTMLParser is a high-performance C++ library designed to parse and manipulate HTML content. It is lightweight, fast, and flexible for developers working with HTML in C++ environments. The library can be used for web scraping, document conversion, and more.  
Modified from [HTMLParser](https://github.com/rangerlee/htmlparser) for providing optimizing performance and bugfix.  

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Built With

* C++
* CMake

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

These instructions will help you get a copy of the project up and running on your local machine.

### Prerequisites

Make sure you have the following installed:

* C++ compiler (e.g., MSVC, GCC or Clang)
* CMake (for building the project)

### Installation

1. Clone the repo:
   ```sh
   git clone https://github.com/cubevlmu/HTMLParser.git
   ```

2. Navigate to the project directory:

   ```sh
   cd HTMLParser
   ```

3. Create a build directory and navigate into it:

   ```sh
   mkdir build
   cd build
   ```

4. Generate the build files using CMake:

   ```sh
   cmake ..
   ```

5. Build the project:

   ```sh
   cmake --build . --config Release --parallel
   ```
   
6. Export the library and headers:

   ```sh
   cmake --install .
   ```
<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

To use HTMLParser, simply include the header file and instantiate the parser:

```cpp
#include "html/HParser.hpp"

int main() {
    std::string html = "<html><body><h1>Hello, world!</h1></body></html>";
    html::HParser parser {};
    html::HDoc doc = parser.parse(html);
    std::cout << "Parsed as text: " << doc.getElementByTagName("h1")[0].getValue() << std::endl;
    return 0;
}
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Roadmap

* [ ] Support for more HTML elements and attributes
* [ ] Error handling improvements
* [ ] Unit tests for parsing functions

See the [open issues](https://github.com/cubevlmu/HTMLParser/issues) for a full list of proposed features and known issues.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

We welcome contributions from the open-source community. Here's how you can help:

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/cubevlmu/HTMLParser/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=cubevlmu/HTMLParser" alt="contrib.rocks image" />
</a>

## License

Distributed under the MIT License. See `LICENSE` for more information.
Original code is under Apache2.0 License. See `docs\HTMLParser-LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact

* [Cubevlmu](https://github.com/cubevlmu)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Acknowledgments

* [HTMLParser](https://github.com/rangerlee/htmlparser) original version of this parser modified from.  
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->

[contributors-shield]: https://img.shields.io/github/contributors/cubevlmu/HTMLParser.svg?style=for-the-badge
[contributors-url]: https://github.com/cubevlmu/HTMLParser/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/cubevlmu/HTMLParser.svg?style=for-the-badge
[forks-url]: https://github.com/cubevlmu/HTMLParser/network/members
[stars-shield]: https://img.shields.io/github/stars/cubevlmu/HTMLParser.svg?style=for-the-badge
[stars-url]: https://github.com/cubevlmu/HTMLParser/stargazers
[issues-shield]: https://img.shields.io/github/issues/cubevlmu/HTMLParser.svg?style=for-the-badge
[issues-url]: https://github.com/cubevlmu/HTMLParser/issues
[license-shield]: https://img.shields.io/github/license/cubevlmu/HTMLParser.svg?style=for-the-badge
[license-url]: https://github.com/cubevlmu/HTMLParser/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
