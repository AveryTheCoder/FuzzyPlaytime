class Player:
    def __init__(self, health, speed, inventory, animation):
        self.health = health  # Health of the player
        self.speed = speed      # Speed of the player
        self.inventory = inventory  # Inventory of items
        self.animation = animation  # Animation of the player

    def __str__(self):
        return f"Player(health={self.health}, speed={self.speed}, inventory={self.inventory}, animation='{self.animation}')"