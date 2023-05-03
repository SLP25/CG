import numpy as np
import matplotlib.pyplot as plt

from collections import namedtuple

Point = namedtuple("Point", "x y")


def calculate_points_farther_density(width: float, height: float, amount: int = 100):

    result: list[Point] = []

    # Define the density inversion parameter.
    d = 0.02

    # Generate angles.
    angles = np.linspace(0, 2 * np.pi, amount)

    # Calculate the semi-major and semi-minor axes for each angle.

    density = np.exp(
        -d * (width * np.sin(angles)) ** 2 / (2 * (width * np.sin(angles)) ** 2 + 4 * (height * np.cos(angles)) ** 2))

    a = width * density
    b = height * density

    # Convert polar coordinates to Cartesian coordinates.
    x = a * np.cos(angles)
    y = b * np.sin(angles)

    # Rotate the coordinates.
    x_rotated = (x - y) / np.sqrt(2)
    y_rotated = (x + y) / np.sqrt(2)

    # Scale the coordinates.
    x_scaled: np.ndarray = x_rotated * 20 / width
    y_scaled: np.ndarray = y_rotated * 5 / height

    for i in range(amount):
        print(f'<point x="{x_scaled[i]}" y="0" z="{y_scaled[i]}" />')
        result.append(Point(x_scaled[i], y_scaled[i]))

    return result, x_scaled, y_scaled


def calculate_points(width: float, height: float, amount: int) -> tuple[list[Point], np.ndarray, np.ndarray]:
    result: list[Point] = []

    # Generate n angles.
    theta = np.linspace(0, 2 * np.pi, amount)

    # Calculate the radius for each angle. -> tuple[list[Point], ]
    r = width * height / np.sqrt((height * np.cos(theta)) ** 2 + (width * np.sin(theta)) ** 2)

    # Convert to Cartesian coordinates.
    x = r * np.cos(theta)
    y = r * np.sin(theta)

    # Rotate the coordinates.
    x_rotated = (x - y) / np.sqrt(2)
    y_rotated = (x + y) / np.sqrt(2)

    # Scale the coordinates.
    x_scaled: np.ndarray = x_rotated * 20 / width
    y_scaled: np.ndarray = y_rotated * 5 / height

    # Print the coordinates.
    for i in range(amount):
        print(f'<point x="{x_scaled[i]}" y="0" z="{y_scaled[i]}" />')
        result.append(Point(x_scaled[i], y_scaled[i]))

    return result, x_scaled, y_scaled


if __name__ == "__main__":

    points, xx, yy = calculate_points_farther_density(
        width=10,
        height=2.5,
        amount=250
    )

    # Plot the points.
    plt.scatter(xx, yy, s=1)

    # Set the axis limits.
    plt.xlim(-30, 30)
    plt.ylim(-26, 26)

    # Set the aspect ratio to match the ellipse.
    plt.gca().set_aspect('equal', adjustable='box')

    # Add axis labels and a title.
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Ellipse')

    # Show the plot.
    plt.show()
