# main.py
import pyray as pr
import Box2D
from config import SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE, FPS, BACKGROUND_COLOR, PPM

class Game:
    def __init__(self):
        pr.init_window(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE)
        pr.set_target_fps(FPS)

        # Physics world
        self.world = Box2D.b2World(gravity=(0, 9.81), doSleep=True)

        # Game objects
        self.ball = None
        self.walls = []
        self.flippers = []

        self._create_boundaries()

    def _create_boundaries(self):
        # Ground
        ground = self.world.CreateStaticBody(position=(0, SCREEN_HEIGHT / PPM))
        ground.CreateEdgeChain(
            [
                (0, 0),
                (SCREEN_WIDTH / PPM, 0),
                (SCREEN_WIDTH / PPM, -SCREEN_HEIGHT / PPM),
                (0, -SCREEN_HEIGHT / PPM),
                (0, 0)
            ]
        )
        self.walls.append(ground)

    def run(self):
        while not pr.window_should_close():
            self.update()
            self.draw()

        pr.close_window()

    def update(self):
        # Physics step
        self.world.Step(1.0 / FPS, 6, 2)
        self.world.ClearForces()

    def draw(self):
        pr.begin_drawing()
        pr.clear_background(BACKGROUND_COLOR)

        # Draw a placeholder message
        pr.draw_text("Pinball Game Under Construction", 190, 200, 20, pr.LIGHTGRAY)

        pr.end_drawing()

if __name__ == '__main__':
    game = Game()
    game.run()
