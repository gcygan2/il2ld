LDN     B2
AND     B3
OR      (
  LDN B1
  AND B2
)
OR      (
  LD    B2
  ANDN  B3
)
ST      H1
LDN     B1
ANDN    B2
OR      B3
ST      H2
LDN     B2
AND     B3
OR (
    LDN     B1
    AND     B2
    )
OR (
    LD      B1
    ANDN    B2
)
ST      H3
LDN     B2
AND     B3
ORN     B1
ST      H4