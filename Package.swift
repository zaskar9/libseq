// swift-tools-version:5.9
// libseq as a Swift package. The C++ library is compiled by SwiftPM (no CMake involved); Swift
// reaches it through Swift's C++ interoperability (module CLibSeq, include/module.modulemap), and
// the module LibSeq gives it a Swift interface. Usage:
//
//     .package(url: "https://github.com/zaskar9/libseq.git", branch: "main")
//     ...
//     .target(name: "App", dependencies: [.product(name: "LibSeq", package: "libseq")],
//             swiftSettings: [.interoperabilityMode(.Cxx)])
//
// and `import LibSeq` in Swift. A module that imports LibSeq needs C++ interoperability too.
import PackageDescription

let package = Package(
    name: "libseq",
    platforms: [.macOS(.v14), .iOS(.v17)],
    products: [
        .library(name: "LibSeq", targets: ["LibSeq"]),
    ],
    targets: [
        .target(
            name: "CLibSeq",
            path: ".",
            sources: ["src"],
            publicHeadersPath: "include"
        ),
        .target(
            name: "LibSeq",
            dependencies: ["CLibSeq"],
            path: "swift/Sources/LibSeq",
            swiftSettings: [.interoperabilityMode(.Cxx)]
        ),
        .testTarget(
            name: "LibSeqTests",
            dependencies: ["LibSeq"],
            path: "swift/Tests/LibSeqTests",
            swiftSettings: [.interoperabilityMode(.Cxx)]
        ),
    ],
    cxxLanguageStandard: .cxx20
)
