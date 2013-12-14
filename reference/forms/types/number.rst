.. index::
   single: Forms; Fields; number

.. note::

    * 対象バージョン：2.4 (2.0以降)
    * 翻訳更新日：2013/12/14

number フィールドタイプ
=======================

テキストフィールドを表示し、数値入力の処理に特化しています。このタイプは精度、端数処理、グループ化についていくつかのオプションを提供しています。

+-------------+----------------------------------------------------------------------+
| 対応するタグ| ``input`` ``text`` フィールド                                        |
+-------------+----------------------------------------------------------------------+
| オプション  | - `rounding_mode`_                                                   |
|             | - `precision`_                                                       |
|             | - `grouping`_                                                        |
+-------------+----------------------------------------------------------------------+
| 継承された  | - `required`_                                                        |
| オプション  | - `label`_                                                           |
|             | - `read_only`_                                                       |
|             | - `disabled`_                                                        |
|             | - `error_bubbling`_                                                  |
|             | - `error_mapping`_                                                   |
|             | - `invalid_message`_                                                 |
|             | - `invalid_message_parameters`_                                      |
|             | - `mapped`_                                                          |
+-------------+----------------------------------------------------------------------+
| 親タイプ    | :doc:`form </reference/forms/types/form>`                            |
+-------------+----------------------------------------------------------------------+
| クラス      | :class:`Symfony\\Component\\Form\\Extension\\Core\\Type\\NumberType` |
+-------------+----------------------------------------------------------------------+

フィールドオプション
--------------------

.. include:: /reference/forms/types/options/precision.rst.inc

rounding_mode
~~~~~~~~~~~~~

**データ型**: ``integer`` **デフォルト**: ``NumberToLocalizedStringTransformer::ROUND_HALFUP``

( ``precision`` オプションに基づいて）送信された数字の端数処理をする必要がある場合、さまざまな端数処理の設定オプションを持ちます。それらは  :class:`Symfony\\Component\\Form\\Extension\\Core\\DataTransformer\\NumberToLocalizedStringTransformer` クラスの定数です。:
    
* ``NumberToLocalizedStringTransformer::ROUND_DOWN`` 0 に向けて丸めます。

* ``NumberToLocalizedStringTransformer::ROUND_FLOOR`` 負の無限大に向かって丸めます。

* ``NumberToLocalizedStringTransformer::ROUND_UP`` 小数点以下切り上げます。

* ``NumberToLocalizedStringTransformer::ROUND_CEILING`` 正の無限大方向に丸めます。

* ``NumberToLocalizedStringTransformer::ROUND_HALF_DOWN`` 「もっとも近い数字」 に丸めます。両方から等距離にある場合、切捨てします。

* ``NumberToLocalizedStringTransformer::ROUND_HALF_EVEN`` 「もっとも近い数字」 に丸めます。両方から等距離にある場合、末尾が偶数のほうに丸めます。

* ``NumberToLocalizedStringTransformer::ROUND_HALF_UP`` 「もっとも近い数字」 に丸めます。両方から等距離にある場合、切り上げます。

.. include:: /reference/forms/types/options/grouping.rst.inc

継承されたオプション
--------------------

以下のオプションは :doc:`form </reference/forms/types/form>` タイプを継承しています:

.. include:: /reference/forms/types/options/required.rst.inc

.. include:: /reference/forms/types/options/label.rst.inc

.. include:: /reference/forms/types/options/read_only.rst.inc

.. include:: /reference/forms/types/options/disabled.rst.inc

.. include:: /reference/forms/types/options/error_bubbling.rst.inc

.. include:: /reference/forms/types/options/error_mapping.rst.inc

.. include:: /reference/forms/types/options/invalid_message.rst.inc

.. include:: /reference/forms/types/options/invalid_message_parameters.rst.inc

.. include:: /reference/forms/types/options/mapped.rst.inc

.. 2013/12/14 yositani2002 b1478f3c6a4e4d8bca7a8d1f6c56a9eec95a266f
