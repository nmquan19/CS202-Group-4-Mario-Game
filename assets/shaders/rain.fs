// Pixel-art rain over scrolling hills with parallax + twinkling pixel stars
// Everything (rain, splashes, trails, ground textures, stars) is in world space.

// ---------- utils ----------
vec3 hsv2rgb(vec3 c){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0,4,2),6.0)-3.0)-1.0,0.0,1.0);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

float hash11(float p){
    p = fract(p*0.1031);
    p *= p + 33.33;
    p *= p + p;
    return fract(p);
}

float hash21(vec2 p){
    // tiny 2D hash ➜ [0,1)
    p = fract(p*vec2(0.1031,0.11369));
    p += dot(p, p.yx + 19.19);
    return fract(p.x*p.y);
}

// 1D value noise
float vnoise1(float x){
    float i = floor(x), f = fract(x);
    float a = hash11(i), b = hash11(i+1.0);
    float u = f*f*(3.0-2.0*f);
    return mix(a, b, u);
}

// Ground height (world-space X)
float groundYAt_world(float xWorld,
                      float base, float amp, float freq,
                      float nfreq, float nmix)
{
    float s  = sin(6.2831853 * freq * xWorld);
    float n  = vnoise1(xWorld * nfreq) * 2.0 - 1.0;
    float h  = mix(s, n, nmix);
    return base + amp * h;
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    // ---------- coords / time ----------
    vec2  uv     = fragCoord.xy / iResolution.xy; // screen [0,1]
    float aspect = iResolution.x / iResolution.y;
    float t      = iTime;

    // ---------- accumulators ----------
    vec3  colAccum    = vec3(0.0);
    float alphaAccum  = 0.0;
    float borderAccum = 0.0;

    // ---------- knobs ----------
    // Rain
    int   COLS      = 80;
    float baseDrop  = 0.005;
    float borderW   = 0.001;
    float splashRel = 0.05;

    // Trails
    float trailGap  = 0.30;
    float trailLife = 0.50;
    float trailLag  = 0.70;
    float shrinkFac = 0.80;
    int   MAX_EMIT  = int(ceil(1.0 / trailGap));

    // Splashes
    int   N_SPL        = 5;
    float g            = 4.0;
    float tScale       = 0.75;
    float postS        = 0.30;
    float splashSpread = 0.65; // 0 = narrow, 1 = wide

    // Foreground ground shape (world-space)
    float groundBase = 0.055;
    float groundAmp  = 0.025;
    float groundFreq = 1.3;
    float noiseFreq  = 3.0;
    float noiseMix   = 0.60;

    // Grass
    float grassBand  = 0.010;
    float grassTuftW = 0.020;
    float tuftProb   = 0.65;
    float tuftHeight = 0.015;

    // Camera / parallax speeds (world units per second)
    float hillSpeedFG = -0.03;              // foreground/world scroll
    float hillSpeedBG = hillSpeedFG * 0.35; // slower parallax ridge

    // Pixel snap (vertical) for ground layers (higher = chunkier steps)
    float groundSnap = 200.0;

    // ---- Stars (world-space) ----
    // Star grid cell size in world UV (larger = fewer cells to test)
    float starCellW      = 0.0125;
    float starCellH      = 0.025;
    float starDensity    = 0.05;  // chance a cell contains a star
    float starParallax   = hillSpeedFG * 0.12; // super-slow drift vs FG
    float twinkleSpeed   = 2.2;   // Hz-ish twinkle rate
    float twinkleDepth   = 0.35;  // 0..1 brightness modulation depth
    float colorVar       = 0.75;  // hue & saturation variation (0..1)

    // 1px clamp for splash & star sizes
    float px = 1.0 / min(iResolution.x, iResolution.y);

    // ---------- camera & world mapping ----------
    float camX_FG = -hillSpeedFG * t;  // camera offset for FG/world
    float camX_BG = -hillSpeedBG * t;  // camera offset for BG/world
    float camX_ST = -starParallax * t; // camera offset for far-star layer
    float worldX  = uv.x + camX_FG;    // fragment's FG world-space X

    // ---------- STARFIELD (draw into background sky first) ----------
    vec3 bg = vec3(0.02, 0.02, 0.035); // night sky base

    // Convert screen pixel to star-layer world coords
    float starWX = uv.x + camX_ST;
    float starWY = uv.y; // stars don't need vertical parallax

    // Identify which cell we're in (world-space), then test a 3x3 neighborhood
    vec2 cell = floor(vec2(starWX / starCellW, starWY / starCellH));
    vec3 starCol = vec3(0.0);

    for (int oy = -1; oy <= 1; ++oy){
        for (int ox = -1; ox <= 1; ++ox){
            vec2 c = cell + vec2(float(ox), float(oy));
            float r = hash21(c);
            if (r > starDensity) continue; // empty cell

            // Per-star randoms
            float r2 = hash21(c + 17.0);
            float r3 = hash21(c + 37.0);

            // Star position inside cell (world)
            vec2 starPosW = (c + vec2(r, r2)) * vec2(starCellW, starCellH);
            // Project to screen (subtract camera)
            vec2 starPosS = vec2(starPosW.x - camX_ST, starPosW.y);

            // Size in pixels: 1..3 px (quantized)
            float sizePx = 1.0 + floor(r3 * 1.5);
            float halfUV = 0.5 * sizePx * px; // convert px to UV half-size

            // Twinkle: each star has its own phase & speed
            float phase  = r * 6.2831853 + r2 * 4.0;
            float tw     = 1.0 - twinkleDepth + twinkleDepth * (0.5 + 0.5 * sin(phase + t * twinkleSpeed * (0.6 + 0.8*r3)));
            // Color: slight hue (blue→white→yellow), low saturation
            float hue    = mix(0.08, 0.10, r);                   // 0.58≈blue ➜ 0.10≈gold
            float sat    = 0.05 + 0.20 * (r2 * colorVar);
            float val    = 0.80 + 0.20 * r3;
            vec3  scol   = hsv2rgb(vec3(hue, sat, val)) * tw;

            // Draw square star (pixel-crisp)
            vec2 d = abs(uv - starPosS);
            float starPix = step(d.x, halfUV) * step(d.y, halfUV);

            starCol += scol * starPix;
        }
    }

    // Layer stars under ridge/ground; slight vignette so zenith is brighter
    float vign = smoothstep(1.0, 0.4, uv.y);
    bg += starCol * (0.85 + 0.15 * vign);

    // ---------- RAIN COLUMNS (world-indexed) ----------
    float worldColsPerUnit = float(COLS);
    float colWidthW        = 1.0 / worldColsPerUnit;
    int   firstIndex       = int(floor(camX_FG * worldColsPerUnit)); // world col at screen-left

    for (int i = 0; i < COLS; i++)
    {
        int   idx         = firstIndex + i;                          // world column index
        float xPosWorld   = (float(idx) + 0.5) * colWidthW;          // world center
        float xPosScreen  = xPosWorld - camX_FG;                     // appears on screen
        float seed        = float(idx) * 17.3;                       // stable randomness

        if (xPosScreen < -0.02 || xPosScreen > 1.02) continue;

        // per-column randomness
        float randOff = fract(sin(seed) * 43758.5453);
        float randSpd = mix(0.4, 0.9, fract(sin(seed*5.11)*417.3));
        float dropS   = baseDrop * mix(0.7, 1.3, fract(sin(seed*11.57)*24567.3));

        // color per column
        float hue = mix(0.50, 0.60, fract(sin(seed*8.23)*43758.5453));
        float sat = 0.6 + 0.2 * fract(sin(seed*3.14)*12345.6789);
        float val = 0.8 + 0.2 * fract(sin(seed*5.77)*98765.4321);
        vec3  dCol= hsv2rgb(vec3(hue, sat, val));

        // local ground height under this column (WORLD X)
        float groundY = groundYAt_world(xPosWorld, groundBase, groundAmp, groundFreq, noiseFreq, noiseMix);
        groundY = floor(groundY * groundSnap) / groundSnap;

        // falling head
        float localT = fract(t*randSpd + randOff); // 0..1
        float yPos   = 1.0 - localT;

        // head square + border, clipped to local ground
        float xDist   = abs(uv.x - xPosScreen) * aspect;
        float yDist   = abs(uv.y - yPos);
        float headPix = step(xDist, dropS) * step(yDist, dropS);
        float headBor = step(xDist, dropS+borderW)*step(yDist, dropS+borderW) - headPix;

        float headVisible = step(groundY, yPos - dropS);
        headPix *= headVisible;
        headBor *= headVisible;

        colAccum    += dCol * headPix;
        alphaAccum  += headPix;
        borderAccum += headBor;

        // trail quads, clipped to ground
        for (int k = 0; k < MAX_EMIT; k++){
            float bornT = float(k) * trailGap;
            float age   = localT - bornT;
            if (age < 0.0 || age > trailLife) continue;

            float ySeg    = yPos + age * randSpd * (1.0 - trailLag);
            float segSize = dropS * (1.0 - shrinkFac * age / trailLife);

            float sx = abs(uv.x - xPosScreen) * aspect;
            float sy = abs(uv.y - ySeg);

            float segPix = step(sx, segSize) * step(sy, segSize);
            float segBor = step(sx, segSize+borderW)*step(sy, segSize+borderW) - segPix;

            float segVisible = step(groundY, ySeg - segSize);
            segPix *= segVisible;
            segBor *= segVisible;

            float fade  = 1.0 - age / trailLife;
            float flick = hash11(seed + float(k)*19.7 + floor(t*60.0)) * 0.5 + 0.5;
            float segA  = fade * flick * 0.8;

            borderAccum += segBor;
            colAccum    += dCol * segPix * segA;
            alphaAccum  += segPix * segA;
        }

        // -------- splashes (tied to last impact at local ground) --------
        float phase        = t * randSpd + randOff;                 // cycles
        float lastImpactT  = (floor(phase) - randOff) / randSpd;    // seconds
        float ageSec       = t - lastImpactT;                       // since impact

        if (ageSec >= 0.0 && ageSec < postS) {
            for (int j = 0; j < N_SPL; j++) {
                float pSeed  = seed + float(j) * 4.7;

                // delay & lifetime (seconds)
                float pDelay = 0.05 + 0.10 * fract(sin(pSeed*1.93) * 413.37);
                float pLife  = mix(1.0, 1.8,  fract(sin(pSeed*5.27) * 935.1));

                float a = ageSec - pDelay;
                if (a < 0.0 || a > pLife) continue;

                // spread-controlled launch (speed + angle)
                float r1    = fract(sin(pSeed * 6.77) * 812.0);      // speed
                float r2    = fract(sin(pSeed * 2.13) * 43758.23);   // angle
                float vMag  = mix(0.28, 0.55, r1);                   // UV/s
                float maxAng= mix(0.05, 0.90, splashSpread);         // radians
                float ang   = (r2 * 2.0 - 1.0) * maxAng;

                float vy = vMag * cos(ang);
                float vx = vMag * sin(ang);

                float dt = a * tScale;
                float pxh = xPosScreen + (vx / aspect) * dt;         // screen-space X
                float pyh = groundY     + (vy * dt - 0.5 * g * dt * dt);

                if (pyh < groundY) continue;

                float sdx = abs(uv.x - pxh) * aspect;
                float sdy = abs(uv.y - pyh);

                float lifeFrac = clamp(a / pLife, 0.0, 1.0);
                float pSize    = max(dropS * splashRel * (1.0 - 0.25 * lifeFrac), px);

                float pPix   = step(sdx, pSize) * step(sdy, pSize);
                float pBor   = step(sdx, pSize+borderW)*step(sdy, pSize+borderW) - pPix;
                float flick  = hash11(pSeed + floor(iTime * 24.0)) * 0.5 + 0.5;
                float fade   = 1.0 - lifeFrac;
                float spA    = pPix * flick * fade;

                borderAccum += pBor;
                colAccum    += dCol * spA;
                alphaAccum  += spA;
            }
        }
    }

    // ---------- BACKGROUND LAYERS ----------
    // Parallax ridge (world-space, slow)
    float ridgeY = groundYAt_world(uv.x + camX_BG,
                                   0.18, 0.045, 0.6,
                                   1.4, 0.50);
    ridgeY = floor(ridgeY * groundSnap) / groundSnap;

    if (uv.y < ridgeY){
        vec3 ridgeCol = vec3(0.071,0.129,0.110);
        float bands = floor(uv.y * 140.0) / 140.0;
        float tex   = hash11(floor((uv.x+camX_BG)*160.0) + floor(bands*999.0)) * 0.03;
        ridgeCol += tex;
        // Stars are UNDER the ridge; overwrite bg with ridge where present
        bg = ridgeCol;
    }

    // Foreground soil+grass from FG world ground line (also scrolls)
    float gy = groundYAt_world(uv.x + camX_FG,
                               groundBase, groundAmp, groundFreq,
                               noiseFreq, noiseMix);
    gy = floor(gy * groundSnap) / groundSnap;

    if (uv.y < gy + grassBand) {
        // Soil base (texture in world-space so it scrolls)
        vec3 soil = vec3(0.078,0.302,0.204);
        float depth = clamp((gy - uv.y) / max(groundAmp + grassBand, 1e-3), 0.0, 1.0);
        soil *= mix(1.0, 0.6, depth);

        float layer = floor(uv.y * 80.0) * 0.07;
        float n = hash11(floor((uv.x+camX_FG)*320.0) + layer * 331.0) * 0.08;
        soil += vec3(n);

        // Grass cap
        vec3 grass = vec3(0.149,0.310,0.149);
        float withinGrass = smoothstep(gy, gy+grassBand, uv.y);

        // Tufts – world-space tiles so they move with ground
        float tileX = floor((uv.x + camX_FG) / grassTuftW);
        float rnd   = hash11(tileX * 57.0);
        if (rnd < tuftProb) {
            float cxWorld = (tileX + 0.5) * grassTuftW;
            float cxScreen= cxWorld - camX_FG;                // tuft center on screen
            float lean    = (hash11(tileX*13.0)-0.5) * 0.008;
            float txs = (uv.x - cxScreen) + (uv.y - gy) * lean;
            float tys = (uv.y - gy);
            float w  = 0.003;
            float blade = step(0.0, tys) * step(tys, tuftHeight) *
                          step(abs(txs), mix(w, 0.0005, tys/tuftHeight));
            grass += vec3(0.10, 0.20, 0.10) * blade;
        }

        // Rim light along grass edge
        float rim = 1.0 - smoothstep(0.0, 0.0025, gy - uv.y);
        grass += vec3(0.10, 0.12, 0.05) * rim;

        bg = mix(soil, grass, withinGrass);
    }

    // ---------- COMPOSITE ----------
    vec3 base  = colAccum / max(alphaAccum, 1e-4);
    vec3 final = mix(bg, base, clamp(alphaAccum, 0.0, 1.0));
    final      = mix(final, vec3(0.886), clamp(borderAccum, 0.0, 1.0) * 0.8);

    fragColor = vec4(final, 1.0);
}
