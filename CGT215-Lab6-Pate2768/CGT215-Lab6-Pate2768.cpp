#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

int main() {
    // Load both images so we can read and change their pixels.
    Image backgroundImage;
    Image foregroundImage;

    if (!backgroundImage.loadFromFile("images1/backgrounds/winter.png")) {
        cout << "Couldn't load background image" << endl;
        return 1;
    }

    if (!foregroundImage.loadFromFile("images1/characters/yoda.png")) {
        cout << "Couldn't load foreground image" << endl;
        return 1;
    }

    // Assume the top-left corner contains only the green screen.
    Color greenScreen = foregroundImage.getPixel(0, 0);

    // Start with the background. Green-screen areas will leave it visible.
    Image compositeImage = backgroundImage;

    Vector2u backgroundSize = backgroundImage.getSize();
    Vector2u foregroundSize = foregroundImage.getSize();

    // Examine the overlapping area so we never access pixels
    // outside either image. Both images are aligned at the top-left.
    for (unsigned int y = 0;
        y < backgroundSize.y && y < foregroundSize.y; y++) {

        for (unsigned int x = 0;
            x < backgroundSize.x && x < foregroundSize.x; x++) {

            Color currentPixel = foregroundImage.getPixel(x, y);

            // Compare the red, green, and blue channels to the
            // sampled green-screen color.
            bool isGreenScreen =
                currentPixel.r == greenScreen.r &&
                currentPixel.g == greenScreen.g &&
                currentPixel.b == greenScreen.b;

            // Keep the character's pixels. For green-screen pixels,
            // leave the background pixel already in compositeImage.
            if (!isGreenScreen) {
                compositeImage.setPixel(x, y, currentPixel);
            }
        }
    }

    RenderWindow window(VideoMode(1024, 768), "Here's the output");
    window.setFramerateLimit(60);

    // Convert the finished image into a texture for drawing.
    Texture compositeTexture;
    if (!compositeTexture.loadFromImage(compositeImage)) {
        cout << "Couldn't create composite texture" << endl;
        return 1;
    }

    Sprite sprite;
	sprite.setTexture(compositeTexture); // Set the sprite to use the composite texture.

    // Process events so the window stays responsive and can close.
    while (window.isOpen()) {
        Event event; 
        while (window.pollEvent(event)) {
			if (event.type == Event::Closed) { // Close the window if the user clicks the close button.
                window.close();
            }
        }

		if (!window.isOpen()) { // Exit the while loop if the window was closed.
            break;
        }

		window.clear(); // Clear the window before drawing the new frame.
		window.draw(sprite); // Draw the sprite with the composite image.
		window.display(); // Display the contents of the window on the screen.
    }

    return 0;
}