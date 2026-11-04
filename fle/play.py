"""Connect to the running FLE server in free (open play) mode and run a snippet."""

import gym

import fle.env.gym_env.registry  # noqa: F401  (registers Factorio-* envs on import)
from fle.env.gym_env.action import Action
from fle.commons.models.game_state import GameState

env = gym.make("open_play", run_idx=0)
obs, info = env.reset(options={"game_state": None})
env.instance.set_speed_and_unpause(1)  # real-time speed, so the client view is watchable

code = 'print(nearest(Resource.IronOre))'
action = Action(
    agent_idx=0,
    game_state=GameState.from_instance(env.instance),
    code=code,
)
obs, reward, terminated, truncated, info = env.step(action)
print(obs)

env.close()
