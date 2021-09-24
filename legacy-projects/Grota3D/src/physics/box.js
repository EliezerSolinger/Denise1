(function(exports) {

    // Vector Class -----------------------------------------------------------
    // ------------------------------------------------------------------------
    function Vector2(x, y) {
        this.x = x;
        this.y = y;
    }

    Vector2.prototype = {

        add: function(v) {
            return new Vector2(this.x + v.x, this.y + v.y);
        },

        sub: function(v) {
            return new Vector2(this.x - v.x, this.y - v.y);
        },

        dot: function(v) {
            return this.x * v.x + this.y * v.y;
        },

        cross: function(v) {
            return this.x * v.y - this.y * v.x;
        },

        div: function(s) {
            this.x /= s;
            this.y /= s;
            return this;
        },

        mul: function(s) {
            this.x *= s;
            this.y *= s;
            return this;
        },

        normalize: function() {

            var len = this.length();
            if (this.length > 0.0001) {
                var invLen = 1.0 / len;
                this.x *= invLen;
                this.y *= invLen;
            }

        },

        // Length
        lengthSqr: function() {
            return this.x * this.x + this.y * this.y;
        },

        length: function() {
            return Math.sqrt(this.x * this.x + this.y * this.y);
        }

    };


    // AABB implementation ----------------------------------------------------
    // ------------------------------------------------------------------------
    function AABB(x, y, w, h, mass) {
        
        mass = mass !== undefined ? mass : 0;

        // Properties
        this.id = ++AABB.id;

        // The is actually the inverse mass.
        // The value 0 is used as a placeholder for infinite mass
        this.im = mass === 0 ? 0 : 1 / mass;

        // How "bouncy" this box is. The higher the value, the more the box will
        // bounce of in case of a collision
        this.restitution = 0.1;

        this.staticFriction = 1;
        this.dynamicFriction = 0.3;

        this.position = new Vector2(x, y);
        this.velocity = new Vector2(0, 0);
        this.size = new Vector2(w, h);

        // Internal, temporary values only used during the collision phase
        this.force = new Vector2(0, 0);
        this.min = new Vector2(0, 0);
        this.max = new Vector2(0, 0);

    }

    AABB.prototype = {

        /**
          * Quick check to see if all of the axis of two AABBs are overlapping.
          */
        isOverlapping: function(other) {
            if (this.max.x < other.min.x || this.min.x > other.max.x) {
                return false;

            } else if (this.max.y < other.min.y || this.min.y > other.max.y) {
                return false;

            } else {
                return true;
            }
        },

        /**
          * Update the boundaries of the AABB
          */
        updateBounds: function() {
            this.min.x = this.position.x - this.size.x;
            this.max.x = this.position.x + this.size.x;
            this.min.y = this.position.y - this.size.y;
            this.max.y = this.position.y + this.size.y;
        },

        /**
          * Integrate force and gravity into the AABB's velocity.
          */
        integrateForces: function(gravity) {
            if (this.im !== 0) {
                this.velocity.x += (this.force.x * this.im + gravity.x) / 2;
                this.velocity.y += (this.force.y * this.im + gravity.y) / 2;
            }
        },

        /**
          * Integrate the velocity into the AABB's position.
          */
        integrateVelocity: function(gravity) {
            if (this.im !== 0) {
                this.position.x += this.velocity.x;
                this.position.y += this.velocity.y;
                this.integrateForces(gravity);
            }
        },

        applyImpulse: function(x, y) {
            this.velocity.x += this.im * x;
            this.velocity.y += this.im * y;
        },

        applyForce: function(x, y) {
            this.force.x += x;
            this.force.y += y;
        },

        clearForces: function() {
            this.force.x = 0;
            this.force.y = 0;
        }

    };


    // Manifold which contains information about a single collision -----------
    // ------------------------------------------------------------------------
    function Manifold(a, b) {

        this.a = a;
        this.b = b;
        this.e = Math.min(a.restitution, b.restitution);
        this.sf = 0;
        this.df = 0;

        this.normal = new Vector2(0, 0);
        this.penetration = 0;

    }

    Manifold.prototype = {

        /**
          * Initialize the manifold for the collision phase.
          */
        init: function(gravity, epsilon) {

            // TODO is this correct?
            this.sf = Math.sqrt(this.a.staticFriction * this.b.staticFriction);
            this.df = Math.sqrt(this.a.dynamicFriction * this.b.dynamicFriction);

            // Figure out whether this is a resting collision, if so do not apply
            // any restitution
            var rx = this.b.velocity.x - this.a.velocity.x,
                ry = this.b.velocity.y - this.a.velocity.y;

            if ((rx * rx + ry * ry) < (gravity.x * gravity.x + gravity.y * gravity.y) + epsilon) {
                this.e = 0.0;
            }

        },

        /**
          * Solve the SAT for two AABBs
          */
        solve: function() {

            // Vector from A to B
            var nx = this.a.position.x - this.b.position.x,
                ny = this.a.position.y - this.b.position.y;

            // Calculate half extends along x axis
            var aex = (this.a.max.x - this.a.min.x) / 2,
                bex = (this.b.max.x - this.b.min.x) / 2;

            // Overlap on x axis
            var xoverlap = aex + bex - Math.abs(nx);
            if (xoverlap > 0) {

                // Calculate half extends along y axis
                var aey = (this.a.max.y - this.a.min.y) / 2,
                    bey = (this.b.max.y - this.b.min.y) / 2;

                // Overlap on x axis
                var yoverlap = aey + bey - Math.abs(ny);
                if (yoverlap) {

                    // Find out which axis is the axis of least penetration
                    if (xoverlap < yoverlap) {

                        // Point towards B knowing that n points from A to B
                        this.normal.x = nx < 0 ? 1 : -1;
                        this.normal.y = 0;
                        this.penetration = xoverlap;
                        return true;

                    } else {

                        // Point towards B knowing that n points from A to B
                        this.normal.x = 0;
                        this.normal.y = ny < 0 ? 1 : -1;
                        this.penetration = yoverlap;
                        return true;

                    }

                }

            }

            return false;

        },

        /**
          * Resolves a collision by applying a impulse to each of the AABB's
          * involved.
          */
        resolve: function(epsilon) {

            var a = this.a,
                b = this.b,

                // Relative velocity from a to b
                rx = b.velocity.x - a.velocity.x,
                ry = b.velocity.y - a.velocity.y,
                velAlongNormal = rx * this.normal.x + ry * this.normal.y;

            // If the velocities are separating do nothing
            if (velAlongNormal > 0 ) {
                return;

            } else {

                // Correct penetration
                var j = -(1.0 + this.e) * velAlongNormal;
                j /= (a.im + b.im);

                // Apply the impulse each box gets a impulse based on its mass
                // ratio
                a.applyImpulse(-j * this.normal.x, -j * this.normal.y);
                b.applyImpulse(j * this.normal.x, j * this.normal.y);

                // Apply Friction
                var tx = rx - (this.normal.x * velAlongNormal),
                    ty = ry - (this.normal.y * velAlongNormal),
                    tl = Math.sqrt(tx * tx + ty * ty);

                if (tl > epsilon) {
                    tx /= tl;
                    ty /= tl;
                }

                var jt = -(rx * tx + ry * ty);
                jt /= (a.im + b.im);

                // Don't apply tiny friction impulses
                if (Math.abs(jt) < epsilon) {
                    return;
                }

                if (Math.abs(jt) < j * this.sf) {
                    tx = tx * jt;
                    ty = ty * jt;

                } else {
                    tx = tx * -j * this.df;
                    ty = ty * -j * this.df;
                }

                a.applyImpulse(-tx, -ty);
                b.applyImpulse(tx, ty);

            }

        },

        /**
          * This will prevent objects from sinking into each other when they're
          * resting.
          */
        positionalCorrection: function() {

            var a = this.a,
                b = this.b;

            var percent = 0.7,
                slop = 0.05,
                m = Math.max(this.penetration - slop, 0.0) / (a.im + b.im);

            // Apply correctional impulse
            var cx = m * this.normal.x * percent,
                cy = m * this.normal.y * percent;

            a.position.x -= cx * a.im;
            a.position.y -= cy * a.im;

            b.position.x += cx * b.im;
            b.position.y += cy * b.im;

        }

    };


    // AABB collision engine --------------------------------------------------
    // ------------------------------------------------------------------------
    function Engine() {
        this.iterations = 10;
        this.gravity = new Vector2(0, 1);
        this.contacts = [];
        this.boxes = [];
        this.length = 0;
    }

    Engine.EPSILON = 0.0001;

    Engine.prototype = {

        findCollisions: function() {

            this.contacts.length = 0;

            for(var i = 0; i < this.length; i++) {

                var a = this.boxes[i];
                for(var j = i + 1; j < this.length; j++) {

                    var b = this.boxes[j];

                    // Ignore collisions between objects with infinite mass
                    if (a.im === 0 && b.im === 0) {
                        continue;

                    } else if (a.isOverlapping(b)) {
                        var c = new Manifold(a, b);
                        if (c.solve()) {
                            this.contacts.push(c);
                        }
                    }

                }

            }

        },

        integrateForces: function() {
            for(var i = 0; i < this.length; i++) {
                this.boxes[i].integrateForces(this.gravity);
            }
        },

        initializeCollisions: function() {
            for(var i = 0, l = this.contacts.length; i < l; i++) {
                this.contacts[i].init(this.gravity, Engine.EPSILON);
            }
        },

        solveCollisions: function() {
            for(var i = 0; i < this.iterations; i++) {
                for(var c = 0, l = this.contacts.length; c < l; c++) {
                    this.contacts[c].resolve(Engine.EPSILON);
                }
            }
        },

        integrateVelocities: function() {
            for(var i = 0; i < this.length; i++) {
                this.boxes[i].integrateVelocity(this.gravity);
                this.boxes[i].clearForces();
                this.boxes[i].updateBounds();
            }
        },

        correctPositions: function() {
            for(var i = 0, l = this.contacts.length; i < l; i++) {
                this.contacts[i].positionalCorrection();
            }
        },

        tick: function() {
            this.findCollisions();
            this.integrateForces();
            this.initializeCollisions();
            this.solveCollisions();
            this.integrateVelocities();
            this.correctPositions();
        },

        addBox: function(box) {
            this.boxes.push(box);
            this.length++;
        }

    };

    exports.Engine = Engine;
    exports.Vector2 = Vector2;
    exports.Box = AABB;

})(typeof module === 'undefined' ? window : module.exports);


var box = exports;

var ground = new box.Box(0, 20, 100, 10);
//ground.restitution = 1;
var object = new box.Box(0, -20, 10, 10, 1);
//var two = new box.Box(20, -40, 10, 10, 1);
//two.restitution = 0.1;

var w = new box.Engine();
w.addBox(ground);
w.addBox(object);
//w.addBox(two);

//object.applyImpulse(4, 0);

setInterval(function() {
    //object.applyForce(0.5, 0);
    w.tick();
    console.log(Math.round(object.position.x), object.position.y);

}, 33);
