type basic = {. "children": React.element};
type reactElement('a) = option('a) => React.element;
type override;
type overrides = Js.Dict.t(override);
type options;
type props('a) = reactElement(Js.t('a));
[@bs.module "markdown-to-jsx"]
external compiler: (string, options) => React.element = "";
// Please, help, I'm begging for help...
[%raw
  {|
  Object.defineProperty(window, "__markdown_to_rex__", {
    value: x => x,
    writable: false
  })
|}
];
[@bs.scope "window"] [@bs.val]
external override_of_component: reactElement('a) => override =
  "__markdown_to_rex__";
// The rest is kinda fine tho...
let overrides_of_array = (ovs: array((string, override))): overrides => {
  ovs->Belt.Array.reduce(
    Js.Dict.empty(),
    (d, (n, o)) => {
      d->Js.Dict.set(n, o);
      d;
    },
  );
};
let overrides_of_list = (ovs): overrides =>
  ovs->Array.of_list->overrides_of_array;

let make_options: (overrides, bool, bool) => options = [%bs.raw
  {|
  (ovs, fb, fil) => {
    let o = {}
    if(ovs && typeof ovs === "object" && !Array.isArray(ovs))
      o.overrides = ovs
    if(typeof fb !== "undefined") o.forceBlock = fb
    if(typeof fil !== "undefined") o.forceInline = fil
    return o
  }
|}
];

let parse_interpolated_children: string => string = [%raw
  {|
  x => (typeof x !== "string")
    ? x
    : x.replace(/\s{6,}/g, "\n")
|}
];

[@react.component]
let make =
    (
      ~overrides: option(array((string, override)))=?,
      ~forceBlock: option(bool)=?,
      ~forceInline: option(bool)=?,
      ~children,
    ) => {
  open Helpers;
  let ovs = overrides->vor([||]) |> overrides_of_array;

  let forceBlock = forceBlock->vor(false);
  let forceInline = forceInline->vor(false);
  let options = make_options(ovs, forceBlock, forceInline);

  let c = children->parse_interpolated_children;
  c->Js.Console.log;
  c->compiler(options);
};