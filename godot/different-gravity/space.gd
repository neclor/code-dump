class_name Space extends Node2D


const G: int = 1000


func _physics_process(_delta: float) -> void:
	var atoms: Array = get_children()

	for i: int in atoms.size() - 1:
		for j: int in range(i + 1, atoms.size()):
			var atom_1: Atom = atoms[i]
			var atom_2: Atom = atoms[j]

			var direction: Vector2 = atom_1.global_position.direction_to(atom_2.global_position)

			var force: float = (
				G * atom_1.mass * atom_2.mass /
				(atom_1.global_position - atom_2.global_position)
					.length_squared()
			)

			direction = direction.rotated(-PI / 18)

			atom_1.apply_central_force(direction * force)
			atom_2.apply_central_force(-direction * force)
