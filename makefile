build:
	clang-format -i */*.cpp
	clang-format -i */*.hpp

	xcodebuild -project game.xcodeproj -alltargets

clean:
	rm -rf build