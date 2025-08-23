#include "ConvexHull.h"

std::vector<sf::Vector2f> ConvexHull(const std::vector < sf::Vector2f >& _points)
{
    int n = _points.size();
    int k = 0;

    if (n <= 3)
    {
        return _points;
    }

    std::vector<sf::Vector2f> points = _points;
    std::vector<sf::Vector2f> hull(2 * n);

    std::sort(points.begin(), points.end(), [](const sf::Vector2f& a, const sf::Vector2f& b)
    {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });

    // Build lower hull
    for (int i = 0; i < n; ++i)
    {
        sf::Vector2f a = CalculateVector(hull[k - 2], hull[k - 1]);
        sf::Vector2f b = CalculateVector(hull[k - 2], points[i]);

        while (k >= 2 && CrossProduct(a, b) <= 0)
        {
            k--;
        }
        hull[k++] = points[i];
    }

    // Build upper hull
    for (int i = n - 2, t = k + 1; i >= 0; --i)
    {
        sf::Vector2f a = CalculateVector(hull[k - 2], hull[k - 1]);
        sf::Vector2f b = CalculateVector(hull[k - 2], points[i]);

        while (k >= t && CrossProduct(a, b) <= 0)
        {
            k--;
        }
        hull[k++] = points[i];
    }

    hull.resize(k - 1);
    return hull;
}