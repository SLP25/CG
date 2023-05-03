import numpy
import matplotlib.pyplot as plt

QUADRUPLE_SIZE: int = 4


def num_segments(point_chain: tuple) -> int:
    # There is 1 segment per 4 points, so we must subtract 3 from the number of points
    return len(point_chain) - (QUADRUPLE_SIZE - 1)


def flatten(list_of_lists) -> list:
    # E.g. mapping [[1, 2], [3], [4, 5]] to  [1, 2, 3, 4, 5]
    return [elem for lst in list_of_lists for elem in lst]


def catmull_rom_spline(
        P0: tuple,
        P1: tuple,
        P2: tuple,
        P3: tuple,
        num_points: int,
        alpha: float = 0.5,
):
    """
    Compute the points in the spline segment
    :param P0, P1, P2, and P3: The (x,y) point pairs that define the Catmull-Rom spline
    :param num_points: The number of points to include in the resulting curve segment
    :param alpha: 0.5 for the centripetal spline, 0.0 for the uniform spline, 1.0 for the chordal spline.
    :return: The points
    """

    # Calculate t0 to t4. Then only calculate points between P1 and P2.
    # Reshape linspace so that we can multiply by the points P0 to P3
    # and get a point for each value of t.
    def tj(ti: float, pi: tuple, pj: tuple) -> float:
        xi, yi = pi
        xj, yj = pj
        dx, dy = xj - xi, yj - yi
        l = (dx ** 2 + dy ** 2) ** 0.5
        return ti + l ** alpha

    t0: float = 0.0
    t1: float = tj(t0, P0, P1)
    t2: float = tj(t1, P1, P2)
    t3: float = tj(t2, P2, P3)
    t = numpy.linspace(t1, t2, num_points).reshape(num_points, 1)

    A1 = (t1 - t) / (t1 - t0) * P0 + (t - t0) / (t1 - t0) * P1
    A2 = (t2 - t) / (t2 - t1) * P1 + (t - t1) / (t2 - t1) * P2
    A3 = (t3 - t) / (t3 - t2) * P2 + (t - t2) / (t3 - t2) * P3
    B1 = (t2 - t) / (t2 - t0) * A1 + (t - t0) / (t2 - t0) * A2
    B2 = (t3 - t) / (t3 - t1) * A2 + (t - t1) / (t3 - t1) * A3
    points = (t2 - t) / (t2 - t1) * B1 + (t - t1) / (t2 - t1) * B2
    return points


def catmull_rom_chain(points: tuple, num_points: int) -> list:
    """
    Calculate Catmull-Rom for a sequence of initial points and return the combined curve.
    :param points: Base points from which the quadruples for the algorithm are taken
    :param num_points: The number of points to include in each curve segment
    :return: The chain of all points (points of all segments)
    """
    point_quadruples = (  # Prepare function inputs
        (points[idx_segment_start + d] for d in range(QUADRUPLE_SIZE))
        for idx_segment_start in range(num_segments(points))
    )
    all_splines = (catmull_rom_spline(*pq, num_points, alpha=1) for pq in point_quadruples)
    return flatten(all_splines)


def points(width: float, height: float, amount: int):

    import numpy as np

    # Generate n angles
    theta = np.linspace(0, 2 * np.pi, amount)

    # Calculate the radius for each angle
    r = width * height / np.sqrt((height * np.cos(theta)) ** 2 + (width * np.sin(theta)) ** 2)

    # Convert to Cartesian coordinates
    x = r * np.cos(theta)
    y = r * np.sin(theta)

    # Rotate the coordinates
    x_rotated = (x - y) / np.sqrt(2)
    y_rotated = (x + y) / np.sqrt(2)

    # Scale the coordinates
    x_scaled = x_rotated * 20 / width
    y_scaled = y_rotated * 5 / height

    # Print the coordinates
    for i in range(amount):
        print(f'<point x="{x_scaled[i]}" y="0" z="{y_scaled[i]}" />')


if __name__ == "__main__":

    # POINTS: tuple = (
    #     (10, -10),
    #     (-2.5, -2.5),
    #     (-10, 10),
    #     (2.5, 2.5),
    # )
    #
    # NUM_POINTS: int = 10  # Density of blue chain points between two red points
    #
    # chain_points: list = catmull_rom_chain(POINTS, NUM_POINTS)
    #
    # for point_array in chain_points:
    #     print(f'<point x="{point_array[0]}" y="0" z="{point_array[1]}" />')
    #
    # print("POINTS")
    # for point_array in POINTS:
    #     print(f'<point x="{point_array[0]}" y="0" z="{point_array[1]}" />')
    #
    # assert len(chain_points) == num_segments(POINTS) * NUM_POINTS  # 400 blue points for this example
    #
    # plt.plot(*zip(*chain_points), c="blue")
    # plt.plot(*zip(*POINTS), c="red", linestyle="none", marker="o")
    # plt.plot(0, 0, c="purple", marker="o", linestyle="none")
    # plt.show()

    points(10, 2.5, 500)