from collections import namedtuple


class PixelComparableByCoords:
    x: int
    y: int

    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __lt__(self, obj):
        """self < obj."""
        if self.y == obj.y:
            return self.x < obj.x
        else:
            return self.y < obj.y

    def __le__(self, obj):
        """self <= obj."""
        return self < obj or self == obj

    def __eq__(self, obj):
        """self == obj."""
        return self.x == obj.x and self.y == obj.y

    def __ne__(self, obj):
        """self != obj."""
        return self.x != obj.x or self.y != obj.y

    def __gt__(self, obj):
        """self > obj."""
        if self.y == obj.y:
            return self.x > obj.x
        else:
            return self.y > obj.y

    def __ge__(self, obj):
        """self >= obj."""
        return self > obj or self == obj


Direction = namedtuple('Direction', 'dx dy')
Detour = {key: value for (key, value) in zip(range(8), [
    Direction(dx=1, dy=0),
    Direction(dx=1, dy=-1),
    Direction(dx=0, dy=-1),
    Direction(dx=-1, dy=-1),
    Direction(dx=-1, dy=0),
    Direction(dx=-1, dy=1),
    Direction(dx=0, dy=1),
    Direction(dx=1, dy=1),
])}
ClockwiseTurn90 = {key: value for (key, value) in zip(range(8), [j for j in range(6, 8)] + [j for j in range(6)])}
