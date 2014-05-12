cascade_validation
~~~~~~~~~~~~~~~~~~

**データ型**: Boolean **デフォルト**: false

埋め込んだフォームタイプの検証を強制するためにこのオプションを ``true`` にします。
例えば、 ``ProductType`` に ``CategoryType`` を埋め込んで持つ場合、 ``ProductType`` の ``cascade_validation`` を ``true`` にすることで、 検証されるべき ``CategoryType`` からもデータが発生します。

このオプションの代わりに、モデルの ``Valid`` 制約でプロパティに格納されている子オブジェクトの検証を強制することができます。

.. include:: /reference/forms/types/options/_error_bubbling_hint.rst.inc

.. 2014/05/10 yositani2002 163ad785f105f9dfde603df5ae7e326b0d85df53
