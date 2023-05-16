import numpy as np
import matplotlib.pyplot as plt

from collections import namedtuple

Point = namedtuple("Point", "x y")

def calculate_points_lame(dist: float, speed: float, attraction: float, amount: int):
    delta = 0.01
    result: list[Point] = []

    x = dist
    y = 0
    vx = 0
    vy = speed

    halfway = False
    
    while not halfway or y < 0:
        #calculate acceleration
        d = np.sqrt(x ** 2 + y ** 2)
        a = attraction / d ** 2
        ax = - x * a / d
        ay = - y * a / d

        #update velocity
        vx += ax * delta
        vy += ay * delta

        #update position
        x += vx * delta
        y += vy * delta

        result.append(Point(x, y))

        if y < 0:
            halfway = True

    result = result[::(len(result) // amount)]

    for p in result:
        print(f'<point x="{p.x}" y="0" z="{p.y}" />')

    return result, [p.x for p in result], [p.y for p in result]

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

    points, xx, yy = calculate_points_lame(30, 0.2, 7, 32)

    # Plot the points.
    plt.scatter(xx, yy, s=1)

    # Set the axis limits.
    plt.xlim(-20, 40)
    plt.ylim(-10, 10)

    # Set the aspect ratio to match the ellipse.
    plt.gca().set_aspect('equal', adjustable='box')

    # Add axis labels and a title.
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title('Ellipse')

    # Show the plot.
    plt.show()
